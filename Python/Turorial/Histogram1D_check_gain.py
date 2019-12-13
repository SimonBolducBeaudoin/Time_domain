#!/bin/env/python
#! -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");
from Histograms import *

## Experiment parameters
    # TimeQuad params
l_data= 1<<24;
    # Guzik params
gain_dB = 11.0 ; # Warning : choose this wisely before running

## Loading GUZIK
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()
    
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=True, ext_ref='int')

## The EXPERIMENT
    # Fetching data from Guzik
data = get( gz ).astype('float64'); # En attendant de correctement implémenter les histograms de int16

H = Histogram_uint64_t( 2**16 , 36 );
H.accumulate(data, max = 2**15)

figure();
plot(H.get())

