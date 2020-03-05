#!/bin/python
# -*- coding: utf-8 -*-

from pyHegel import *
from my_python_plots_methods import*

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
   
