#!/bin/env/python
#! -*- coding: utf-8 -*-
import sys, os, ctypes
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/Histograms-OTF/')
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/aCorrs-OTF/bin/')
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/remdet-OTF/bin/')
from acorrs_otf import ACorrUpTo
from histograms_otf import hist1dNbits, hist2dNbits, cumulant, moment
from remdet import getdet, remdet, set_num_threads
from threading import Thread
from itertools import product


# Le but est d'utiliser les histogrammes pour obtenir une courbe de shot-noise photoexcité et de 
# vérifier que les paramètres sont tous adéquats pour la suite

# On va prendre plusieurs mesures courtes qu'on va moyenner par la suite pour
# éviter les dérives temporelles.
# On alterne entre Vdc=/=0 et Vdc=0 pour faire la mesure en différentiel.

# La phase n'est pas gardée constante ici.

# On tente d'être efficace et de réellement traiter les données précédentes pendant l'acquisition 
# de nouvelles données en swappant des arrays en mémoire.


test = False

make_dir("C://Projets/TimeDomain_Guzik/Data/%D/Guzik_PhotoEx_FullBand_PhaseLocked/" + ("test" if test else ""))


# Test de timing: ~26.25 h / 213 iteration
# Aiming for Wednesday February 26th 10:00 108.5 hours:  108.5/526.25*213 = 880.4 sweeps

#######################
## DEVICES VARIABLES ##
#######################
n_measures = 881 if not test else 1
# gz
gz_samples = 8*2**30    # 52.5 GiB is the max value
gz_gain = 7
gz_equalizer = 1
gz_ref = 'ext'
bits_16 = True
# acorrs
num_acorrs = 129 # = 128+1
fft_acorrs = False
phi_acorrs = 16 # 32/2 = 16
fftchunk = 16*num_acorrs # Sweet-spot empirique pour num_acorrs=64, 16 est mieux pour grands délais
# moments
num_moments = 6
hist_bits = 16
# phases
phi_target = 90
delta_phi = 0.5
fac = 2e9
delay_mode = '625ps'

    
#####################
## MANIP VARIABLES ##
#####################

# BIASES
def PdBm2Vac(PdBm):
    return 10**(PdBm/20.)*1./sqrt(10)

def Vac2PdBm(Vac):
    return 10*log10((Vac/sqrt(2))**2/50*1e3)                                             

## Sweeps Vdc @ Vac=cte
vdc_target = r_[0,linspace(5,6,3)]  # Plus grand pour etre plus sur que Vac a peu d'impact
#vdc = zeros(2*len(vdc_target))
#vdc[1::2] = vdc_target
vdc = vdc_target    # No need for differential here
# Previous values
vac = r_[-122, Vac2PdBm(linspace(PdBm2Vac(-15-6),PdBm2Vac(-3-6),5))] # -122 and less -> -inf

vdc_waittime = 0.75 # Seconds

########################
## SETTING UP DEVICES ##
########################

# Loading devices
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        gz = instruments.guzik_adp7104()
except:
    gz = instruments.guzik_adp7104()
gz.config([1], n_S_ch=gz_samples, gain_dB=gz_gain, bits_16=bits_16, equalizer_en=gz_equalizer, ext_ref=gz_ref)
load("yo10")
set(yo10,0)
set(yo10.output_en, True)
set(yo10.range,10)
load("dmm12")
set(dmm12.aperture, .5)
set(dmm12.zero, 1)
load("att1")
set(att1, 101)
rs1 = instruments.rs_sgma('TCPIP::rssgs100a110885.mshome.net::inst0::INSTR')
set(rs1.output_en,False)
set(rs1.power_level_dbm, -20)
set(rs1.freq,fac)
set(rs1.ref_output_signal, 'ref')

def rs1_power_extended_set_wrap(p):
    if p < -121:
        set(rs1.output_en,0)
        return
    elif not get(rs1.output_en):
        set(rs1.output_en,1)
    if p < -20:
        set(att1, 101)
        set(rs1.power_level_dbm, -20+p%1)
        set(att1, -20-p + ceil(p%1))
    else:
        set(rs1.power_level_dbm, p)
        set(att1, 0)
        

def rs1_power_extended_get_wrap():
    if not get(rs1.output_en):
        return -inf
    tmp1 = get(rs1.power_level_dbm)
    tmp2 = get(att1)
    return tmp1-tmp2        
    

rs1_power_extended = instruments.FunctionWrap(setfunc=rs1_power_extended_set_wrap, 
                                              getfunc=rs1_power_extended_get_wrap, 
                                              basedevs=[rs1,att1], 
                                              multi=["rs1.output_level_dbm - att1"]
                                             )    

# Setting output power to minimal value
set(rs1_power_extended, -122)


## Virtual GZ Devices
def get_or_getcache(dev, use_cache, *args, **kwargs):
    if use_cache == 'prev':
        return dev.alias._prev
    elif use_cache:
        try:
            return dev.getcache(*args, **kwargs)
        except:
            return dev.alias.getcache(*args, **kwargs)
    else:
        return get(dev, *args, **kwargs)

def gz_snip_wrap(snipsize=1000, use_cache=False, *args, **kwargs):
    v = get_or_getcache(gz, use_cache)
    snip = v[:snipsize].copy()
    return snip
    
def gz_h_wrap(hist_bits=16, use_cache=False, *args, **kwargs):
    v = get_or_getcache(gz, use_cache);
    h = hist1dNbits(v, hist_bits, *args, **kwargs)
    return h
    
def gz_moment_wrap(k=6, h=None, *args, **kwargs):
    if h is None:
        h = gz_h_wrap(*args, **kwargs)
    return r_[[moment(h,n) for n in range(1, k+1)]]
    
def gz_cumul_wrap(k=6, h=None, *args, **kwargs):
    if h is None:
        h = gz_h_wrap(*args, **kwargs)
    return r_[[cumulant(h,n) for n in range(1, k+1)]]    
    
def gz_ac_wrap(k=64, use_cache=False, *args, **kwargs):
    v = get_or_getcache(gz, use_cache);
    a = ACorrUpTo(k, v, *args, **kwargs)
    return a.res
   
def spectrum_from_acorrs(a, rate=32e9, norm=True, *args, **kwargs):
    w = fftshift(fft.hfft(a))
    f = fftshift(fft.fftfreq(len(w), 1./rate))
    return array([f,w if not norm else abs(w)])   
    
def gz_spec_wrap(k=128, use_cache=False, *args, **kwargs):
    v = get_or_getcache(gz, use_cache);
    a = ACorrUpTo(k, v)
    w = spectrum_from_acorrs(a.res, *args, **kwargs)
    return w

def coherent_spectrum(v, k, repeat_k=8, sr=32e9, polar=False, norm=False, nozero=False, *args, **kwargs):
    #if length is None:
    #    length = len(v)
    #if length%k:
    #    w = v[:length-(length%k)].view()
    #else:
    #    w = v[:length].view()
    #w.shape = (w.shape[0]/k,k)
    #z = w.mean(axis=0)
    w = getdet(v,k)
    z = zeros((repeat_k,len(w)),dtype=w.dtype)
    z[:] = w
    z.shape = len(w)*repeat_k,
    x = fftshift(fft.fft(z))
    x = x if not norm else abs(x)
    f = fftshift(fftfreq(len(x),1./sr))
    if nozero:
        x[where(f==0)]=None
    if polar:
        return x
    else:
        return array([f,x])
        

def gz_cohpart_wrap(k=8, repeat_k=1, remove=False, use_cache=False, *args, **kwargs):
    fct = remdet if remove else getdet
    v = get_or_getcache(gz, use_cache);
    w = fct(v,k)
    z = zeros((repeat_k,len(w)),dtype=w.dtype)
    z[:] = w
    z.shape = len(w)*repeat_k,
    return z
    
def gz_cohspec_wrap(k=8, repeat_k=8, use_cache=False, *args, **kwargs):
    v = get_or_getcache(gz, use_cache);
    w = coherent_spectrum(v, k, repeat_k=repeat_k, *args, **kwargs)
    return w
    
def coherent_cw(v, k, f0, *args, **kwargs):
    kwargs.pop("polar",0)
    ff,cc = coherent_spectrum(v,k,polar=False,*args,**kwargs)
    idx = argmin(abs(ff-f0))
    f,c = ff[idx].real, cc[idx]
    if not isclose(f,f0):
        print("{:.6f} MHz selected instead of {:.6f} MHz".format(f/1.e6,f0/1.e6))
    return c

def gz_cohcw_wrap(f0=4e9, k=128, out_fmt="complex", use_cache=False, *args, **kwargs):
    v = get_or_getcache(gz, use_cache);
    w = coherent_cw(v, 2*k, f0, *args, **kwargs)
    if out_fmt.lower()=="complex":
        return w
    elif out_fmt.lower() in ["realimag","ri"]:
        return r_[w.real,w.imag]
    elif out_fmt.lower() in ["rtheta", "rt"]:
        return r_[abs(w),angle(w)]
    else:
        raise KeyError("Invalid output format. Valid values are 'complex', 'realimag', and 'rtheta'.")

def gz_r_wrap(*args, **kwargs):
    kwargs.pop('out_fmt', 0)
    r,t = get(gz_cohcw, out_fmt='rt', *args, **kwargs)
    return r
    
def gz_theta_wrap(*args, **kwargs):
    kwargs.pop('out_fmt', 0)
    r,t = get(gz_cohcw, out_fmt='rt', *args, **kwargs)
    return t    
       

## The actual virtual instruments
gz_snip = instruments.FunctionWrap(getfunc=gz_snip_wrap, basedev=gz.fetch, multi=('GZ_Snippet',))
gz_h = instruments.FunctionWrap(getfunc=gz_h_wrap, basedev=gz.fetch, multi=('GZ_Histogram',))
gz_moment = instruments.FunctionWrap(getfunc=gz_moment_wrap, basedev=gz.fetch, multi=list('GZ_Moments',))
gz_cumul = instruments.FunctionWrap(getfunc=gz_cumul_wrap, basedev=gz.fetch, multi=list('GZ_Cumulants',))
gz_ac = instruments.FunctionWrap(getfunc=gz_ac_wrap, basedev=gz.fetch, multi=('GZ_ACorrs',))
gz_spec = instruments.FunctionWrap(getfunc=gz_spec_wrap, basedev=gz.fetch, multi=('GZ_Spectrum',))
gz_spec._format['xaxis']=True
gz_cohpart = instruments.FunctionWrap(getfunc=gz_cohpart_wrap, basedev=gz.fetch, multi=('GZ_Coherent_Part',))
gz_cohspec = instruments.FunctionWrap(getfunc=gz_cohspec_wrap, basedev=gz.fetch, multi=('GZ_Coherent_Spectrum',))
gz_cohspec._format['xaxis']=True
gz_cohcw = instruments.FunctionWrap(getfunc=gz_cohcw_wrap, basedev=gz.fetch, multi=('GZ_Coherent_CW',))
gz_cohcw_r = instruments.FunctionWrap(getfunc=gz_r_wrap, basedev=gz.fetch, multi=('GZ_Coherent_CW_R',))
gz_cohcw_theta = instruments.FunctionWrap(getfunc=gz_theta_wrap, basedev=gz.fetch, multi=('GZ_Coherent_CW_Theta',))

def _gz_cummul_getformatfunc(**kwargs):
    hist_bits = kwargs.pop('hist_bits',6)
    fmt = gz_cumul._format
    fmt.update(multi=['C{k:d}'.format(k=i) for i in range(1,hist_bits+1)])
    return super(instruments.FunctionWrap, gz_cumul).getformat(**kwargs)

gz_cumul._getformatfunc = _gz_cummul_getformatfunc

def _gz_moment_getformatfunc(**kwargs):
    hist_bits = kwargs.pop('hist_bits',6)
    fmt = gz_moment._format
    fmt.update(multi=['M{k:d}'.format(k=i) for i in range(1,hist_bits+1)])
    return super(instruments.FunctionWrap, gz_moment).getformat(**kwargs)

gz_moment._getformatfunc = _gz_moment_getformatfunc

delay1 = instruments.colby_pdl_100a('GPIB0::2::INSTR')
set(delay1.mode, delay_mode)
set(delay1,312.5 if get(delay1.mode)=="625ps" else 156.25)
def gz_phi_wrap(phi=-90, f0=fac, delta=delta_phi, k=phi_acorrs, mode=delay_mode,verbose=True, max_retry=10, *args, **kwargs):
    if mode == "625ps":
        epsilon = 1
        dt = .5
        tmax = 625  # Much more?
    else:
        epsilon = .5
        dt = .25
        tmax = 312.5
    theta = get(gz_cohcw_theta, k=k, f0=f0, *args, **kwargs)*180./pi
    if phi == None:
        return theta
    d = theta-phi
    #print theta, ' ', d
    for i in range(max_retry):
        if abs(d) > delta:
            d0 = get_or_getcache(delay1, use_cache=False)
            target = (d0+(d/360.)*(epsilon*1e12/f0))
            #target = target-target%.25
            target = dt*round(target/dt)
            #print "theta = ", theta, " | d = " , d, " | d0 = ", d0, " | target = ", target
            if target < 0:
                target += (epsilon*1e12/f0)
            if target > tmax:
                target -= (epsilon*1e12/f0)
            set(delay1, target)
            snap([delay1, yo10, rs1_power_extended], 'delay1_change.log')
            kwargs.pop('use_cache', False)
            #print d0, ' ' , target
            
            theta = get(gz_cohcw_theta, k=k, f0=f0, *args, **kwargs)*180./pi
            
            if verbose:
                print "\nCorrected Phase on {} | delta = {} deg | retry {}".format(time.strftime("%m-%d %H:%M:%S"), d, i+1)
            
            d = theta-phi
        
    if abs(theta-phi) > delta:
        print "!WARNING!: PHASE ADJUSTMENT FAILED", ' | ', 'Theta = ', theta
    return theta #get_or_getcache(gz, True, *args, **kwargs)

gz_phi = instruments.FunctionWrap(getfunc=gz_phi_wrap, basedev=gz.fetch, multi=('GZ_PHASE',))

# To swap data with previous data
get(gz) # Populates gz.fetch._cache)
gz.fetch._prev = zeros(gz.fetch._cache.shape, dtype=int16 if bits_16 else uint8)
gz.fetch._current = gz._gsa_data
def swap_cache():
    gz.fetch._prev,gz.fetch._current = gz.fetch._current,gz.fetch._prev
    gz._gsa_data_res_arr[0].common.data.arr = gz.fetch._current.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
    gz._gsa_data = gz.fetch._current


#####################
## Data structures ##
#####################

respath = get(sweep.path)
savez_compressed(os.path.join(respath, 'dc_biases{}.npz'.format('_test' if test else '')), vdc)
savez_compressed(os.path.join(respath, 'ac_biases{}.npz'.format('_test' if test else '')), vac)

acorr_shape = (len(vac), len(vdc))
acorr0_res_shape = (len(vac), len(vdc), num_acorrs)
acorr_res_shape = (len(vac), len(vdc), phi_acorrs, num_acorrs)
acorr = r_[ [ACorrUpTo(num_acorrs, 'int16', phi=phi_acorrs, fft=fft_acorrs, fftchunk=fftchunk) for i in range(prod(acorr_shape))] ]
acorr.shape = acorr_shape
acorr0_res = zeros(acorr0_res_shape, dtype=double)
acorr_res = zeros(acorr_res_shape, dtype=double)

#moments_res_shape = (n_measures, len(vac), len(vdc), num_moments)
#moments_res = zeros(moments_res_shape, dtype=double)

dmm_res_shape = (n_measures, len(vac), len(vdc))
dmm_res = zeros(dmm_res_shape, double)

detpart_res_shape = (n_measures, len(vac), len(vdc), phi_acorrs)
detpart_res = zeros(detpart_res_shape, dtype=int16 if bits_16 else uint8) # Coherent Spectrum
last_delay = array([312.5,]*len(vac)) # Initial value, then it'll be the last valid one.

######################
## Helper functions ##
######################

# n=n_measures, a=vac, d=vdc

def null():
    pass
    
# For manualy async of gz and dmm12
def dmm_helper(n,a,d):
    dmm_res[n,a,d] = get(dmm12)

def get_data(n,a,d):
    current_phi = phi_target
    # For Vac=0
    if get(rs1_power_extended)<-121:
        current_phi = None
    thread_dmm = Thread(target=dmm_helper, args=(n,a,d))
    thread_dmm.start()
    get(gz_phi, phi=current_phi, f0=fac, k=phi_acorrs, delta=delta_phi, max_retry=50, verbose=False)
    thread_dmm.join()

    last_delay[a] = get_or_getcache(delay1, use_cache=True) # Caching for next loop

def process_previous_data(n,a,d):
    set_num_threads(65) # 72 is faster, but we leave threads for the acquisition
    # Extracting deterministic part and removing it from cached data used afterwards
    detpart_res[n,a,d] = get(gz_cohpart, k=phi_acorrs, remove=True, use_cache='prev')
    # Computing aCorrs
    acorr[a,d](get_or_getcache(gz, use_cache='prev'))
    # Computing histogram and saving its moments
    #moments_res[n,a,d] = get(gz_moment, use_cache='prev', k=num_moments, hist_bits=hist_bits)
    
def save_data(baktime=0):
    # Getting the actual results from ACorrUpToPhi objects
    acorr_res[:] = r_[map(lambda x: x.res, acorr.reshape(prod(acorr.shape)))].reshape(acorr_res_shape) # [:] will update global acorr_res
    acorr0_res[:] = r_[map(lambda x: x.res0, acorr.reshape(prod(acorr.shape)))].reshape(acorr0_res_shape) # [:] will update global acorr0_res
    
    # Saving results in case there's an issue, overwriting at each iteration
    save(os.path.join(respath, 'acorr_res{}.npy'.format('_test' if test else '')), acorr_res)
    save(os.path.join(respath, 'acorr0_res{}.npy'.format('_test' if test else '')), acorr0_res)
    #save(os.path.join(respath, 'moments_res{}.npy'.format('_test' if test else '')), moments_res)
    save(os.path.join(respath, 'dmm_res{}.npy'.format('_test' if test else '')), dmm_res)
    save(os.path.join(respath, 'detpart{}.npy'.format('_test' if test else '')), detpart_res)
    
    # Ensuring we didn't F up the data saving of acorrs with [:]
    tmp = np.load(os.path.join(respath, 'acorr_res{}.npy'.format('_test' if test else '')))
    if not any(nonzero(tmp)):
        raise ValueError('Saved Data is null, did you properly retrieve the result before saving?')        
    
    # Keeping a backup version every 6 hours in case there's a real bad issue
    currtime = time.time()
    if currtime - baktime > 6*3600:
        baktime = time.time()
        tstamp = time.strftime('%Y%m%d_%H%M%S')
        
        savez_compressed(os.path.join(respath, 'acorr_res{}_bak_{}.npz'.format('_test' if test else '', tstamp)), acorr_res)
        savez_compressed(os.path.join(respath, 'acorr0_res{}_bak_{}.npz'.format('_test' if test else '', tstamp)), acorr0_res)
        #savez_compressed(os.path.join(respath, 'moments_res{}_bak_{}.npz'.format('_test' if test else '', tstamp)), moments_res)
        savez_compressed(os.path.join(respath, 'dmm_res{}_bak_{}.npz'.format('_test' if test else '', tstamp)), dmm_res)
        savez_compressed(os.path.join(respath, 'detpart_res{}_bak_{}.npz'.format('_test' if test else '', tstamp)), detpart_res)
    return baktime    

###################
## MEASUREMENTS! ##
###################

# Ensure it's ok
set(yo10.range,max(abs(vdc)))
set(yo10,0) # Just in case
set(rs1_power_extended,-122)

##############
## GO GO GO ##
##############


# The Measurement itself
n_width = "{:d}".format(int(log10(100))+1)
baktime = 0

# Initial setup
thread_processing = Thread(target=null) # First iteration there's nothing to process
thread_processing.start()
baktime = time.time()
# n=n_measures, a=vac, d=vdc
for current_iteration, (n,a,d) in enumerate(product(range(n_measures),range(len(vac)),range(len(vdc)))): # was nlm
    it = time.time()
    # 1 - Let's set AC power and delay if it needs to change!
    if a+d==0:
        print ("\n\nMeasuring iteration {:0"+n_width+"d}/{:0"+n_width+"d}").format(n+1, n_measures)
    if d==0:
        set(rs1_power_extended,vac[a])
        if not get(rs1_power_extended) < -121:
            set(delay1, last_delay[a])
        #print(("  vac: {:02.2f} dBm").format(get(rs1_power_extended)))
        print(" ")
    
    set(yo10,vdc[d])
    pause(vdc_waittime)  # Waiting until voltage is stable
    
    # 2 - Data Acquisition!
    iat = time.time()
    get_data(n,a,d) # Captures the GIL
    fat = time.time()
    # 3 - Ensuring data processing is done before swapping cache
    thread_processing.join()
    swap_cache()
    fpt = time.time()
    # 4 - Saving and Starting data processing
    # Pac loop is done processing after thread_processing on the next iteration is done.
    if current_iteration and (a==0):    # Exception for first iteration
        baktime = save_data(baktime)
    thread_processing = Thread(target=process_previous_data, args=(n,a,d))
    thread_processing.start()
    ft = time.time()
    print ("{:0"+n_width+"d}/{:0"+n_width+"d}").format(n+1, n_measures) + " | vac: {:07.2F} dBm | vdc: {:07.4f} V | {:.1f} GiSa: {:05.2F} sec | SET: {:04.3F} sec | CALC: {:.1F} | TOT: {:04.3F} sec".format(get(rs1_power_extended),get(yo10),gz_samples/2**30, fat-iat, iat-it, fpt-fat, ft-it)
# 5 - Final setup
thread_processing.join()
swap_cache()
baktime = save_data(baktime)
# 6 - Saving final results compressed
acorr_res[:] = r_[map(lambda x: x.res, acorr.reshape(prod(acorr.shape)))].reshape(acorr_res_shape) # [:] will update global acorr_res
acorr0_res[:] = r_[map(lambda x: x.res0, acorr.reshape(prod(acorr.shape)))].reshape(acorr0_res_shape) # [:] will update global acorr0_res
savez_compressed(os.path.join(respath, 'acorr_res{}.npz'.format('_test' if test else '')), acorr_res)
savez_compressed(os.path.join(respath, 'acorr0_res{}.npz'.format('_test' if test else '')), acorr0_res)
#savez_compressed(os.path.join(respath, 'moments_res{}.npz'.format('_test' if test else '')), moments_res)
savez_compressed(os.path.join(respath, 'dmm_res{}.npz'.format('_test' if test else '')), dmm_res)
savez_compressed(os.path.join(respath, 'detpart{}.npz'.format('_test' if test else '')), detpart_res)

# POST EXPERIMENT 
set(yo10,0)
set(rs1_power_extended, -122)
set_num_threads(72) # So the system stays responsive in interactive mode    


