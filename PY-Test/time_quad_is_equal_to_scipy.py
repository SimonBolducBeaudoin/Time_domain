#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from Time_quadratures import *

####################################
## Setup variables
l_kernel = 1<<8 + 1 ;
l_data= 1<<14;
l_fft = 1<<10;
n_threads = 72 ;
convolution_type = 'fft'; # Default value
####################################
############# Declare Time_Quad objects
X  = TimeQuad_double_int16_t( l_kernel, l_data, l_fft, convolution_type , n_threads );

# Numpy instance of arrays
data = array(X.data, copy = False);
kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);
p_full = array(X.p_full, copy = False);
q_full = array(X.q_full, copy = False);
#############

# Generate random data
mu=0
sigma=2**12
data[:]  = int16( normal(mu,sigma,l_data) );

#### Using scipy and python library
print "Calculation with Scipy"
(NoyauP,NoyauQ) = Kernels_time(l_kernel, dt)   # [ns^-1/2]
P_i_scipy = fftconvolve(NoyauP,data,mode='full') # shape 9001
Q_i_scipy = fftconvolve(NoyauQ,data,mode='full')
####

#### Using Time_Quad
print "executing"
X.execute();
####

#### Diplay results
print "ploting"
fig, axs = subplots(3,1)
SLICE = slice( None, None, None );
axs[0].plot(P_i_scipy[SLICE])#;axs[0,1].plot(Q_i_scipy);
axs[1].plot(p_full[SLICE])#;axs[1,1].plot(q_full);
axs[2].plot( (P_i_scipy[SLICE]-p_full[SLICE]) )
#axs[1,0].hist(P_i_scipy,100);axs[1,1].hist(Q_i_scipy,100);
####