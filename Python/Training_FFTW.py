#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from Fast_convolution import *

####################################
## Setup variables
l_kernel = 1<<8;
l_data= 1<<28;
l_fft = 1<<9;
n_threads = 1;
convolution_type = 'fft';
####################################
# L_FFTs = 2**array([9,10,11,12,13,14,15]); 
L_FFTs = 2**array([9,10,11,12,13,14,15,16]); 

# Executions_f = zeros( (size(N_threads) , size(L_FFTs)) );
# Executions_d = zeros( (size(N_threads) , size(L_FFTs)) );

X_f  = FastConvolution_float_float_int16_t_complex_f( l_kernel, l_data, l_fft, convolution_type , n_threads );
X_d  = FastConvolution_double_double_int16_t_complex_d( l_kernel, l_data, l_fft, convolution_type , n_threads );

# X_f.time_execution = 0 ;
# X_d.time_execution = 0 ;

for j, l_fft in enumerate(L_FFTs):
    print "l_fft = {}".format(l_fft);
    X_f.l_fft = l_fft;
    X_d.l_fft = l_fft;

del X_f
del X_d
