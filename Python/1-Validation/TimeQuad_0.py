#!/bin/python
# -*- coding: utf-8 -*-

"""
Validates that 
    - TimeQuad Object can be construsted
    - Pointers can be passed properly in between C++ and python
    - Outputs looks ok
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py")

from time_quadratures import *

## Setup variables
l_kernel = (1<<8) + 1 ;
n_kernels = 1 ;
l_data= (1<<14) - 256;
dt = 0.03125 ;
f_max_analogue = 10.0 ;
f_min_analogue = 0.5 ;
alpha = 0.25 ;
l_fft = 1<<10;
n_threads = 2 ;

X  = TimeQuad(l_kernel=l_kernel , n_kernels=n_kernels , l_data=l_data , dt=dt , f_max_analogue=f_max_analogue , f_min_analogue=f_min_analogue , alpha=alpha , l_fft=l_fft , n_threads=n_threads);

ks_p = X.ks_p();
ks_q = X.ks_q();
ps  = X.ps();
qs  = X.qs();

mu=0
sigma=2**12
data  = int16( normal(mu,sigma,l_data) );

X.execute( data );

fig, axs = subplots(2,1)
axs[0].plot( ps[0,:] )
axs[1].plot( qs[0,:] )
