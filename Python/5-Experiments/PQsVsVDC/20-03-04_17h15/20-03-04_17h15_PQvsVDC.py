#!/bin/env/python
#! -*- coding: utf-8 -*-

"""
Simplpe script that 
    - aquires data from guzik
    - calculates p and q for many input filters
    - Accumulates histograms of ps and qs
    - Calculates culants
    - Displays expval of n and n^^2
"""

################################
## TO FILL AT SCRIPT CREATION ##
################################
time_stamp_start = time.strftime('%y-%m-%d_%Hh%M')
path_test = "C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\TEST" # Common test folder for all experiments

test = False ; # By default test should be true
path_script = "C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\PQsVsVDC" ;
path_current = path_script if not(test) else path_test
filename = "PQvsVDC.py"

make_dir(path_current + "\\" + time_stamp_start ) # pyHegel's function to create dir and changes sweep.path to point there
path_save = get(sweep.path); # The script is going to be saved at the same location as wher pyHegel saves stuff.

########################
## APPEND PARRENT DIR ##
########################
import os, sys
parentdir = os.path.abspath(os.path.join(os.getcwd(), os.pardir)) ;
sys.path.append(parentdir)

#############
## IMPORTS ##
#############
execfile("../common_header.py");
from pyHegel import *
from my_python_library import *
from my_pyHegel_library import *
from my_python_plots_methods import*

from time_quadratures import *
from Histograms import *

#################
## SCRIPT COPY ##
#################
"""
FUTUR ? : Also copy script from my custom librarie to where the data are saved
"""

def Copy_script(path_original,path_copy):
    """
    example :
    path_original = C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\PQsVsVDC\\PQsvsVDC.py
    path_copy = C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\TEST\\PQsvsVDC.py
    """
    with open(path_original, 'r') as f:
        with open(path_copy, 'w') as out:
            for line in (f.readlines()): #remove last 7 lines
                out.write(line)

Copy_script( path_script+"\\"+ filename , path_save+"\\"+ time_stamp_start + "_" + filename )

#############################
## EXPERIMENTAL PARAMETERS ##
#############################
# Kernels params
n_kernels = 4 ;
l_kernel = (1<<8) + 1 ; 
# Acquisition params
dt = 0.03125 ;    
l_data= (1<<28) ;
f_max_analogue = 10.0 ;
f_min_analogue = 0.5 ;
# Filters params
n_filters = 3 ; 
l_hc = l_kernel//2 + 1 ;
df = 0.1 ;
f1 = DFT_freq( 8 , l_dft = l_kernel , dt = dt );
f2 = DFT_freq( 32 , l_dft = l_kernel , dt = dt );
f3 = DFT_freq( 64 , l_dft = l_kernel , dt = dt );
# Windowing params
alpha = 0.75 ;
# Algorithm params
l_fft = 1<<10;
n_threads = 36 ; # Warning the number of thread used might be changed by TimeQuad class and Histogram class
# Guzik params
gain_dB = 11.0

# SWEEP PARAMETERS 
Vdc_target = r_[linspace(-4,-1.2,6),linspace(-1.2,1.2,21)[1:-1],linspace(1.2,4,6)] if not test else r_[linspace(-4,-1.2,2),linspace(-1.2,1.2,2)[1:-1],linspace(1.2,4,2)]
Vdc = Vdc_target

# LOOP PARAMETERS 
n_measures = 2*325 if not test else 5 ;

########################
## INITIALIZING STUFF ##
########################
print "\n Filter list :"
Filters = empty( (n_filters,l_hc) , dtype=complex , order='C' ) ;
Filters[0,:] = Gaussian_filter( f1 , df=df , l_kernel = l_kernel , dt = dt ) ;
Filters[1,:] = Gaussian_filter( f2 , df=df , l_kernel = l_kernel , dt = dt ) ;
Filters[2,:] = Gaussian_filter( f3 , df=df , l_kernel = l_kernel , dt = dt ) ;
print "TimeQuad object : "
X = TimeQuad(l_kernel=l_kernel , n_kernels=n_kernels , l_data=l_data , dt=dt , f_max_analogue=f_max_analogue , f_min_analogue=f_min_analogue ,  filters=Filters , alpha=alpha , l_fft=l_fft , n_threads=n_threads);
ks_p = X.ks_p();
ks_q = X.ks_q();
ps  = X.ps();
qs  = X.qs();
print "Histograms :"
max = 4.5*10**5 ; # first estimate the width of p and q distributions
nb_of_bin = 2**10 ;
Hs_shape = (len(Vdc) , n_kernels ) ;
Hs = r_[[Histogram2D_uint64_t(nb_of_bin,n_threads) for i in range(prod(Hs_shape))]] ;
Hs.shape = Hs_shape ;
H_x = Hs[0,0].abscisse(max) ;
print "Guzik :"
gz = load_guzik() ;
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=True, ext_ref='int')
print "Yoko :"
load("yo10")
set(yo10,0)
set(yo10.output_en, True)
set(yo10.range,10)

#############
## GENERAL ##
#############
def get_or_getcache(dev, use_cache, *args, **kwargs):
    if use_cache:
        try:
            return dev.getcache(*args, **kwargs)
        except:
            return dev.alias.getcache(*args, **kwargs)
    else:
        return get(dev, *args, **kwargs)  

####################
## GUZIK SPECIFIC ##
####################
def load_guzik():
    try:
        if not isinstance(gz, instruments.guzik.guzik_adp7104):
            print "\nLoading guzik :"
            gz = instruments.guzik_adp7104()
    except:
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
        
    return gz
    
def gz_quick_hist(use_cache=False):
    H = Histogram_uint64_t( 2**16 , 36 );
    
    data = get(gz_snippet, snipsize=10**6).astype('float64'); # En attendant de correctement implémenter les histograms de int16
    H.accumulate(data, max = 2**15) ;
    
    quick_plot_gz_hist1D( [H.get()] )

####################
## YOKO SPECIFIC  ##
####################


#######################
## WRAPPER FUNCTIONS ##
#######################
"""
    They are used to define virtual instruments using PyHegel's function wrap method.
    They necessitate that some dev already exist in the global namespace
    They also necessitate the call of instruments.FunctionWrap
"""
## GUZIK SPECIFIC ##
def gz_snippet_wrapper( snipsize=1000):
    v = get(gz)
    snip = v[:snipsize].copy()
    return snip

## YOKO SPECIFIC  ##  
def yo_wait_set_wrap( V , Waittime = 0.4 ):
    set(yo10,V)
    pause(Waittime)  # Waiting until voltage is stable
    
def yo_wait_get_wrap():
    tmp = get(yo10)
    return tmp

########################
## VIRTUAL INSTRUMENTS STUFF ##
########################

gz_snippet = instruments.FunctionWrap(getfunc=gz_snippet_wrapper, basedev=gz.fetch, multi=('GZ_Snippet',))
yo_wait = instruments.FunctionWrap(setfunc=yo_wait_set_wrap , getfunc=yo_wait_get_wrap, basedev=yo10, multi=('Yo_wait',))

########################
## MEASURING STUFF ##
########################
set(yo10.range,abs(Vdc).max())
set(yo10,0) # Just in case

print (" n_threads : {:02}/{:02d}").format(n_threads,72)
t_begin = time.time(); 
for n_measure in range(n_measures) :
    print ("\n Measure {:03}/{:03d}").format(n_measure+1, n_measures)
    for j , vdc in enumerate( Vdc ):
        print ("\n vdc = {:.3F} progess {:03}/{:03d}").format(vdc,j,len(Vdc))
        t0 = time.time(); 
        set(yo_wait, vdc) ;
        t1 = time.time(); 
        data = get( gz );# int16
        t2 = time.time();
        
        X.execute( data );
        t3 = time.time();
        
        for i in range(n_kernels) :
            Hs[j,i].accumulate( ps[i,:] , qs[i,:] , max = max ) ;

        t4 = time.time(); 
        
        print "   Yoko_wait:{:04.2F} [s] |Data Acquisition:  {:04.2F}  [s] |  Quadrature calculations :  {:04.2F}  [s] |  Histogram accumulation: {:.2F}  [s] ".format( t1-t0, t2-t1, t3-t2 ,t4-t3)
t_end = time.time(); 

set(yo10,0) # Just in case

Estimated_time_per_loop = 184.93 ;
print("Experience over : \n \t Estimated time [s] {:04.2F} \n \t Real time {:04.2F} [s] \n ").format( n_measures*Estimated_time_per_loop , t_end-t_begin )

#######################################################
## CONVERT AND COPY ARRAY OF HISTOGRAMS TO BIG ARRAY ##
#######################################################
HS = r_[ [Hs.reshape(prod(Hs_shape))[i].get() for i in range(prod(Hs_shape)) ] ]
HS_shape = (len(Vdc) , n_kernels , nb_of_bin , nb_of_bin ) ;
HS = HS.reshape( HS_shape ) ;

##################
## SAVING STUFF ##
##################
def save_data(path_save):    
    """
    Futur write a quick_save_data(path_save)
        that saves in .npy
    """
    time_stamp = time.strftime('%y-%m-%d_%Hh%M') # File name will correspond to when the experiment ended
    
    savez_compressed(os.path.join(path_save, 'Filters{}_{}.npz'.format('' if not(test) else '_test', time_stamp)), Filters)
    savez_compressed(os.path.join(path_save, 'ks_p{}_{}.npz'.format('' if not(test) else '_test', time_stamp)), ks_p)
    savez_compressed(os.path.join(path_save, 'ks_q{}_{}.npz'.format('' if not(test) else '_test', time_stamp)), ks_q)
    savez_compressed(os.path.join(path_save, 'HS{}_{}.npz'.format('' if not(test) else '_test', time_stamp)), HS) 
    savez_compressed(os.path.join(path_save, 'Vdc{}_{}.npz'.format('' if not(test) else '_test', time_stamp)), Vdc)
    
save_data(path_save);

###################
## ANALYZE STUFF ##
###################
height = 8;

## histogram 1D
def plot_Hist_1D( Hs , Vdc , axis = 0 , x_label="" , y_label="", TR_comment="" ,height = 7 , Ratio_hw = 1.62 ):
    if len(Hs.shape)!= 1:
        raise ValueError('Make sure Hs is one dimensional before calling this funciton. \n Try Hs[:,0]')
    if len(Vdc.shape)!= 1:
        raise ValueError('Make sure Vdc is one dimensional before calling this funciton. \n Try Vdc[:,0]')
        
    fig , axs = subplots(1,1)
    fig.set_size_inches( Ratio_hw*height , height )
    fig.text(0.74, 0.915, TR_comment , fontsize=9)
    for H,vdc in zip(Hs,Vdc) :
        label = "{:2.3F} [V]".format(vdc)
        axs.plot( H.get().sum(axis=axis) , label = label ) ;
    axs.set_ylabel(" Counts ")
    axs.set_xlabel(" bin #")
    axs.legend();
    return fig , axs
        
def plot_Hist_1D_difference( Hs , Vdc , axis = 0 , x_label="" , y_label="", TR_comment="" ,height = 7 , Ratio_hw = 1.62 ):
    if len(Hs.shape)!= 1:
        raise ValueError('Make sure Hs is one dimensional before calling this funciton. \n Try Hs[:,0]')
    if len(Vdc.shape)!= 1:
        raise ValueError('Make sure Vdc is one dimensional before calling this funciton. \n Try Vdc[:,0]')
        
    fig , axs = subplots(1,1)
    fig.set_size_inches( Ratio_hw*height , height )
    fig.text(0.74, 0.915, TR_comment , fontsize=9)
    for H0, H1, vdc in zip(Hs[0:-1:2],Hs[1:-1:2],Vdc[1:-1:2]) :
        label = "{:2.3F} [V]".format(vdc)
        difference = H1.get().sum(axis=axis).astype('float64')-H0.get().sum(axis=axis).astype('float64') ;
        axs.plot( difference , label = label ) ;
    axs.set_ylabel(" Counts ")
    axs.set_xlabel(" bin #")
    axs.legend();
    return fig , axs
    
def plot_Hist_2D_difference( Hs , Vdc , x_label="p" , y_label="q", TR_comment="" ,height = 7 , Ratio_hw = 1.62 ):
    if len(Hs.shape)!= 1:
        raise ValueError('Make sure Hs is one dimensional before calling this funciton. \n Try Hs[:,0]')
    if len(Vdc.shape)!= 1:
        raise ValueError('Make sure Vdc is one dimensional before calling this funciton. \n Try Vdc[:,0]')
    if Hs.size != 2:
        raise ValueError('This function only does the difference between one pair of 2D histograms. \n ')
    if Vdc.size != 2:
        raise ValueError('.Should have the same shape and size has Hs \n Try Vdc[:,0]')
    
    fig , axs = subplots(1,1)
    fig.set_size_inches( Ratio_hw*height , height )
    fig.text(0.74, 0.915, TR_comment , fontsize=9)
    
    label = "{:2.3F} [V]".format(Vdc[1])
    difference = Hs[1].get().astype('float64')-Hs[0].get().astype('float64') ;
    im = axs.imshow( difference ,cmap='coolwarm', interpolation='none' ) ;
    axs.set_title(label)
    colorbar(im);
    
    axs.set_ylabel(x_label)
    axs.set_xlabel(y_label)
    axs.legend();
    return fig , axs
        
# Checks all the Vdc == 0 
# Hs_V0 = Hs[0:-1:2,:]
# plot_Hist_1D(Hs_V0, Vdc[0:-1:2], axis = 0)# should be p 
# plot_Hist_1D(Hs_V0, Vdc[0:-1:2], axis = 1)# should be q

# See the histogram contribution of the junction
# plot_Hist_1D_difference( Hs[:,0] , Vdc )
# plot_Hist_1D_difference( Hs[:,1] , Vdc )
# plot_Hist_1D_difference( Hs[:,2] , Vdc )
# plot_Hist_1D_difference( Hs[:,3] , Vdc )


# histogram 2D
# plot_Hist_2D_difference( Hs[0:2,0] , Vdc ) # 4V - 0V 0th filter
# plot_Hist_2D_difference( Hs[0:2,1] , Vdc ) # 4V - 0V 1st filter
# plot_Hist_2D_difference( Hs[0:2,2] , Vdc ) # 4V - 0V 2nd filter
# plot_Hist_2D_difference( Hs[0:2,3] , Vdc ) # 4V - 0V 3rd filter

## Traduction en python des fonctions écrite en C++
## pour appliquer à HS pas Hs
def moment2D(Hist,exp_x,exp_y ,first_bin_center , bin_width):
    """
    Only works for square histograms
    """ 
    nofbins = Hist.shape[0] ;
    bin_value_x = 0 ;
    bin_value_y = 0 ;
    val = 0.0 ;
    n = Hist.sum() ;
    for i in range(Hist.shape[0]) :
        for j in range(Hist.shape[1]) :
            bin_value_y = bin_width*(j) + first_bin_center ;
            bin_value_x = bin_width*(i) + first_bin_center ;
            val += Hist[i,j] * bin_value_x**exp_x * bin_value_y**exp_y ;
    return val/n ;
    
def centered_moment2D(Hist,exp_x,exp_y,first_bin_center , bin_width):
    """
    Only works for square histograms
    """
    mu_x = moment2D(Hist,1,0 ,first_bin_center , bin_width) ;
    mu_y = moment2D(Hist,0,1 ,first_bin_center , bin_width) ;
      
    nofbins = Hist.shape[0] ;
    bin_value_x = 0 ;
    bin_value_y = 0 ;
    val = 0.0 ;
    n = Hist.sum() ;
    for i in range(Hist.shape[0]) :
        for j in range(Hist.shape[1]) :
            bin_value_y = bin_width*(j) + first_bin_center ;
            bin_value_x = bin_width*(i) + first_bin_center ;
            val += Hist[i,j] * ( bin_value_x - mu_x )**exp_x * ( bin_value_y - mu_y )**exp_y ;
    return val/n ;

# Calcules des moyennes en x et y
def All_moment2D(Hist , exp_x , exp_y , first_bin_center , bin_width , Vdc , n_kernels ):
    avg = empty( (len(Vdc) , n_kernels) , dtype=float ) ;
    for i in range(len(Vdc)):
        for j in range(n_kernels) :
            avg[i,j] = moment2D(Hist , exp_x , exp_y , first_bin_center , bin_width )
    return avg ;
    
def All_centered_moment2D(Hist , exp_x , exp_y , first_bin_center , bin_width , Vdc , n_kernels ):
    avg = empty( (len(Vdc) , n_kernels) , dtype=float ) ;
    for i in range(len(Vdc)):
        for j in range(n_kernels) :
            avg[i,j] = moment2D(Hist , exp_x , exp_y , first_bin_center , bin_width )
    return avg ;
