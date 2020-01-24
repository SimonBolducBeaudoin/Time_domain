#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from Time_quadratures import *

####################################
## Setup variables
l_kernel = 1<<8;
l_data= 1<<14;
l_fft = 1<<10;
n_threads = 72 ;
convolution_type = 'fft'; # Default value
####################################

####################################
X  = TimeQuad_double_int16_t( l_kernel=l_kernel, l_data=l_data, l_fft=l_fft, convolution_type=convolution_type , n_threads=n_threads );

# Numpy instance of arrays (shared memory)
data = array(X.data, copy = False);
kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);
p_full = array(X.p_full, copy = False);
q_full = array(X.q_full, copy = False);

# Generate random data
mu=0
sigma=2**12
data[:]  = int16( normal(mu,sigma,l_data) );

dt = 0.03125; # [ns]; # Default value
(NoyauP,NoyauQ) = Kernels_time(l_kernel, dt); 

error_p = zeros(p_full.size);
error_q  = zeros(p_full.size);
Repetitions =1000
for i in arange(Repetitions):
    data[:]  = int16( normal(mu,sigma,l_data) );
    P_i_scipy = fftconvolve(NoyauP,data,mode='full') # shape 9001
    Q_i_scipy = fftconvolve(NoyauQ,data,mode='full')
    X.execute();
    error_p += absolute(P_i_scipy - p_full);
    error_q += absolute(Q_i_scipy - q_full);


figure(1)
plot(error_p);
figure(2)
plot(error_q);

####################################

# fig, axs = subplots(3,1)
# SLICE = slice( None, None, None );
# axs[0].plot(Scipy[SLICE]);
# axs[1].plot(output_full[SLICE]);
# axs[2].plot( (Scipy[SLICE] - output_full[SLICE] ) );

del X