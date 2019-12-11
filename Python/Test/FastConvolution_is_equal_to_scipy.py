#!/bin/python
# -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");

# Dont forget to drop your library.pyd in the current folder
from Convolution import *

from my_python_library import *
####################################
## Setup variables
l_kernel = (1<<8) + 1;
l_data= 1<<20;
dt = 0.03125; # [ns];
l_fft = 1<<10;
n_threads = 2 ;
####################################

# Generate random data
mu=0
sigma=2**8
data  = normal(mu,sigma,l_data) ;


(kernel,NoyauQ) = Kernels(l_kernel, dt); del NoyauQ;

# ipython.magic("time Scipy = fftconvolve(kernel,data,mode='full') ");
A = fftconvolve(kernel,data,mode='full');

B = Convolution_direct(kernel, data);
C = Convolution_rapide(kernel, data , l_fft, n_threads);

####################################

fig, axs = subplots(5,1)
SLICE = slice( None, 10000, None );
axs[0].plot(A[SLICE]);
axs[1].plot(B[SLICE]);
axs[2].plot(C[SLICE]);
axs[3].plot( (B[SLICE] - A[SLICE] ) );
axs[4].plot( (C[SLICE] - B[SLICE] ) );
