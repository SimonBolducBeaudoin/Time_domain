#!/bin/env/python
#! -*- coding: utf-8 -*-

"""
test :
    - aquires a single time from guzik
    - calculates p and q for many input filters
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");

from pyHegel import *
from my_python_library import *
from my_pyHegel_library import *
from my_python_plots_methods import*

from time_quadratures import *
from Histograms import *

test = True

#############################
## EXPERIMENTAL PARAMETERS ##
#############################
# Kernels params
n_kernels = 2 ;
l_kernel = (1<<8) + 1 ; 
# Acquisition params 
dt = 0.03125 ;  
l_data= (1<<14) ;
# Filters params
n_filters = 1 ; 
l_hc = l_kernel//2 + 1 ;
df = 0.1 ;
f1 = DFT_freq( 8 , l_dft = l_kernel , dt = dt );

# Algorithm params
n_threads = 36 ; # Warning the number of thread used might be changed by TimeQuad class and Histogram class
# Guzik params
gain_dB = 11.0

# SWEEP PARAMETERS 
Vdc = r_[linspace(-1.2,1.2,3)]

# LOOP PARAMETERS 
n_measures = 1 ;

########################
## INITIALIZING STUFF ##
########################
print "\n Filter list :"
Filters = empty( (n_filters,l_hc) , dtype=complex , order='C' ) ;
Filters[0,:] = Gaussian_filter( f1 , df=df , l_kernel = l_kernel , dt = dt ) ;
print "TimeQuad object : "
X = TimeQuad(l_kernel=l_kernel , n_kernels=n_kernels , l_data=l_data , dt=dt ,  filters=Filters , alpha=0.75 , n_threads=n_threads);
ks_p = X.ks_p();
ks_q = X.ks_q();
ps  = X.ps();
qs  = X.qs();
print "Histograms :"
max = 10.0*10**5; # first estimate the width of p and q distributions
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

print  "Measurments :"
data = get( gz );# int16

X.execute( data );

figure(); 
plot(ps[0,:][:1000]) ;
plot(qs[0,:][:1000]) ;
figure(); 
plot(ps[1,:][:1000]) ;
plot(qs[1,:][:1000]) ;

set(yo10,0) # Just in case
