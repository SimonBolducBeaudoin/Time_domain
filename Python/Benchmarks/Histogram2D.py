#!/bin/env/python
#! -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");
from Histograms import *

# You must first recompile the histogram C++ code and manually enable accumulation_timer by decommenting some code.

l_data = (10**9)
std_dev = 1.0;
max = 6*std_dev ; 
# x = array(normal(0,std_dev ,l_data), dtype = 'float64');
# y = array(normal(0,std_dev ,l_data), dtype = 'float64');

NOFBINS = range(8,12+1,1);
N_THREADS = range(1,72+1,1);

# NOFBINS = range(8,9+1,1);
# N_THREADS = range(1,2+1,1);

AccumulationTimes = zeros( (size(NOFBINS) * size(N_THREADS) ) ) ;

header = [ "{}\n Histogram2D_uint64_t \n data type = double \n l_data = 10**9 of 2 independent normal distributions \n\n".format(date.today()),];
texte = header + ['index \t n_threads \t nofbins  \t Time [s] \t GSa/s \n',] ;

print texte ;

for i, (nofbins,n_threads) in enumerate( product(NOFBINS,N_THREADS) ):
  
    H = Histogram2D_uint64_t(nofbins = 2**nofbins , n_threads = n_threads); 
    H.accumulate_timer = 0 ;
    H.accumulate(x,y,max);
    
    tmp_time = H.accumulate_timer/(10.0**6) ; #[sec]
    SamplePerSec = (l_data/tmp_time)/(10.0**9); #[GSa/s]
    
    AccumulationTimes[i] = tmp_time;
    tmp_text = [ "{:03d} \t {:02d} \t\t 2**{:02d} \t\t {:0.3f} \t\t {:0.3f} \n".format(i,n_threads,nofbins, tmp_time , SamplePerSec ) ,]; 
    
    print tmp_text ;
    texte += tmp_text ; 
    
    del H

filename = 'Histogram2D_uint64_t'
with open(filename+".txt", 'w') as f:
    f.writelines(texte) 
    
savez(filename+".npz", header = header , l_data = l_data , 
NOFBINS = NOFBINS ,
N_THREADS = N_THREADS,
AccumulationTimes = AccumulationTimes ) ;