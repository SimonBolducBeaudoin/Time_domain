#!/bin/env/python
#! -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");
from time_quadratures import *
from Histograms import *


L = (10**6)
std_dev = 1.0;
x = array(normal(0,std_dev ,L), dtype = 'float64');

H = Histogram_uint64_t(nofbins = 2**16 , n_threads = 36); 
max = 6*std_dev ; 
H_abscisse = H.abscisse( max );

H.accumulate(x,max);

## check for run conditions problems in histogram
print " look for run conditions problems ".format( H.get().sum() == L)

# height = 8;
# fig = figure()
# fig.set_size_inches( 1.62*height , height )
# plot( H_abscisse, H.get() )


