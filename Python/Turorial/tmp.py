#!/bin/env/python
#! -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)
# sys.path.insert(0,parentdir) ;

test = True ;
datadir = os.path.abspath(os.path.join(currentdir, "%D/data/"+ ("test" if test else ""))) ;
print "\nMake data directory :"
make_dir(datadir);

execfile("../common_header.py");

## VARIABLES
gz_samples = 8e9 if not test else 1e8    # 52.5 GiB is the max value
gz_gain = 11 # 
gz_equalizer = 1
gz_ref = 'int'
bits_16=True

## Loading devices

try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()
    
gz.config([1], n_S_ch=gz_samples, gain_dB=gz_gain, bits_16=bits_16, equalizer_en=gz_equalizer, ext_ref=gz_ref)

def gz_snippet_wrapper(snipsize=1000):
    v = get(gz)
    snip = v[:snipsize].copy()
    return snip
    
gz_snippet = instruments.FunctionWrap(getfunc=gz_snippet_wrapper, basedev=gz.fetch, multi=('GZ_Snippet',))

# The EXPERIMENT
data = get( gz_snippet );

figure()
plot(data)

