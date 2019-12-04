#!/bin/python
# -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py")

# Dont forget to drop your library.pyd in the current folder
from time_quadratures import *

####################################
## Setup variables
l_kernel = (1<<8) + 1 ;
l_data= 1<<14;
l_fft = 1<<10;
n_threads = 2 ;
dt = 0.03125 ;
####################################
############# Declare Time_Quad objects
X  = TimeQuad_int16_t( l_kernel = l_kernel, l_data = l_data , l_fft = l_fft , n_threads = n_threads );

# Numpy instance of arrays
data = array(X.data, copy = False);
kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);
p = array(X.p, copy = False);
q    = array(X.q, copy = False);
#############

# Generate random data
mu=0
sigma=2**12
data[:]  = int16( normal(mu,sigma,l_data) );

#### Using scipy and python library
print "Calculation with Scipy"
# (NoyauP,NoyauQ) = Kernels(l_kernel, dt)   # [ns^-1/2]
P_i_scipy = fftconvolve(kernel_p,data,mode='valid') # shape 9001
Q_i_scipy = fftconvolve(kernel_q,data,mode='valid')
####

#### Using Time_Quad
print "executing"
X.execute();
####

#### Diplay results
print "ploting"
fig, axs = subplots(3,1)
axs[0].plot(P_i_scipy)#;axs[0,1].plot(Q_i_scipy);
axs[1].plot(p)#;axs[1,1].plot(q_full);
axs[2].plot( (P_i_scipy-p) )
#axs[1,0].hist(P_i_scipy,100);axs[1,1].hist(Q_i_scipy,100);
####