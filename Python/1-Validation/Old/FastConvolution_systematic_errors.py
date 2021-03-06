#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from Fast_convolution import *

####################################
## Setup variables
l_kernel = 1<<8;
l_data= 1<<14;
l_fft = 1<<10;
n_threads = 72 ;
convolution_type = 'fft'; # Default value
####################################

####################################
# X  = FastConvolution_float_float_int16_t_complex_f( l_kernel, l_data, l_fft, convolution_type , n_threads );
X  = FastConvolution_double_double_int16_t_complex_d( l_kernel, l_data, l_fft, convolution_type , n_threads );

# Numpy instance of arrays (shared memory)
data = array(X.data, copy = False);
kernel = array(X.kernel, copy = False);
output_full = array(X.output_full, copy = False);
# output_valid = array(X.output_valid, copy = False);

# Generate random data
mu=0
sigma=2**12

dt = 0.03125; # [ns]; # Default value
(kernel[:],NoyauQ) = Kernels_time(l_kernel, dt); del NoyauQ;

error = zeros(output_full.size);
Repetitions =10000
for i in arange(Repetitions):
    data[:]  = int16( normal(mu,sigma,l_data) );
    Scipy = fftconvolve(kernel,data,mode='full');
    X.execute();
    error += absolute(Scipy - output_full);

SLICE = slice( None, None, None );   
figure
plot(error[SLICE]);

####################################

# fig, axs = subplots(3,1)
# SLICE = slice( None, None, None );
# axs[0].plot(Scipy[SLICE]);
# axs[1].plot(output_full[SLICE]);
# axs[2].plot( (Scipy[SLICE] - output_full[SLICE] ) );

del X