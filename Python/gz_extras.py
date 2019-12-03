#!/bin/env/python
#! -*- coding: utf-8 -*-
import sys, os
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/Histograms-OTF/')
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/aCorrs-OTF/bin/')
sys.path.append('C:/Projets/TimeDomain_Guzik/Code/remdet-OTF/bin/')
from acorrs_otf import ACorrUpTo
from histograms_otf import hist1dNbits, hist2dNbits, cumulant, moment
from remdet import getdet, remdet, set_num_threads

from time import strftime


# Virtual GZ Devices
def get_or_getcache(dev, use_cache, *args, **kwargs):
    if use_cache:
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
   
# def _0_spectrum_from_acorrs(a, rate=32e9):
    # aa = r_[a,a[-2:0:-1]] # Symmétrise les autocorrélations
    # w = fftshift(fft.fft(aa))
    # f = fftshift(fft.fftfreq(len(aa), 1./rate))
    # return array([ff,w])   

# def _1_spectrum_from_acorrs(a, rate=32e9):
    # w = fftshift(fft.hfft(a))
    # f = fftshift(fft.fftfreq(len(w), 1./rate))
    # return array([ff,w])      
    
# def spectrum_from_acorrs(a, rate=32e9):
    # x,y = _0_spectrum_from_acorrs(a,rate)
    # u,z = _1_spectrum_from_acorrs(a,rate)
    # return array([x,y,z])
    
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
set(delay1.mode, "312.5ps")
set(delay1,150)
def gz_phi_wrap(phi=-90, f0=4.0e9, delta=2, k=8, *args, **kwargs):
    if phi == None:
    return nan
    theta = get(gz_cohcw_theta, k=k, *args, **kwargs)*180./pi
    d = theta-phi
    #print theta, ' ', d
    max_retry = 5
    for i in range(max_retry):
        if abs(d) > delta:
            d0 = get(delay1)
            target = (d0+(d/360.)*(0.5e12/f0))
            #target = target-target%.25
            target = 0.25*round(target/0.25)
            if target < 0:
                target += (0.5e12/f0)
            if target > 312.5:
                target -= (0.5e12/f0)
            set(delay1, target)
            kwargs.pop('use_cache', False)
            #print d0, ' ' , target
            
            theta = get(gz_cohcw_theta, *args, **kwargs)*180./pi
            
            print "\nCorrected Phase on {} | delta = {} deg | retry {}".format(strftime("%m-%d %H:%M:%S"), d, i+1)
            
            d = theta-phi
        
    if abs(theta-phi) > delta:
        print "!WARNING!: PHASE ADJUSTMENT FAILED", ' | ', 'Theta = ', theta
    return theta #get_or_getcache(gz, True, *args, **kwargs)

gz_phi = instruments.FunctionWrap(getfunc=gz_phi_wrap, basedev=gz.fetch, multi=('GZ_PHASE',))

# RS1 EXTRAS:
def rs1_power_extended_set_wrap(p):
    if p < -20:
        set(att1, 101)
        set(rs1.power_level_dbm, -20)
        set(att1, -20-p)
    else:
        set(rs1.power_level_dbm, p)
        set(att1, 0)
        
def rs1_power_extended_get_wrap():
    tmp1 = get(rs1.power_level_dbm)
    tmp2 = get(att1)
    return tmp1-tmp2

rs1_power_extended = instruments.FunctionWrap(setfunc=rs1_power_extended_set_wrap, 
                                              getfunc=rs1_power_extended_get_wrap, 
                                              basedevs=[rs1,att1], 
                                              multi=["rs1.output_level_dbm - att1"]
                                             )    

# Convenient polar scope for coherent_spectrum/coherent_cw
# Typical uses: 
#   >>> scope_polar(gz_cohspec, polar=True, nozero=True, interval=0.01, k=8, repeat_k=8)
#   >>> scope_polar(gz_cohcw, f0=4e9, interval=0.01, k=128, fftchunk=16*128)
def scope_polar(dev, interval=0.1, fmt='.-', alpha=0.75, *args, **kwargs):
    fig = figure()
    ax = fig.add_subplot(111,projection='polar')
    polar = kwargs.pop("polar", True)
    try:
        d = get(dev, polar=polar, *args, **kwargs)
    except KeyError:
        d = get(dev, *args, **kwargs)
    # First plot
    if isinstance(d,complex):
        line_mode=False
        p, = ax.plot([0,angle(d)], [0,abs(d)], fmt, alpha=alpha)
    else:
        line_mode=True
        p, = ax.plot(angle(d), abs(d), fmt, alpha=alpha)

    while True:
        wait(interval)
        try:
            try:
                d = get(dev, polar=True, *args, **kwargs)
            except KeyError:
                d = get(dev, *args, **kwargs)
            if not line_mode:
                p.set_data([0,angle(d)],[0,abs(d)])
            else:
                p.set_data(angle(d),abs(d))
            ax.relim()
            ax.autoscale_view(True,True,True)
            fig.canvas.draw()
            fig.canvas.flush_events()
        except:
            # TODO: Detect that fig was closed
            break