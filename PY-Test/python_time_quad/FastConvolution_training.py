#!/bin/python
# -*- coding: utf-8 -*-

execfile("headers.py")

####################################
## Setup variables
l_kernel = 1<<8;
l_data= 1<<12;
l_fft = 1<<10;
n_threads = 1;
convolution_type = 'fft'; # Default value

####################################

# N_threads = array([1,2,4,8,16,32,64,72]);
# L_FFTs = 2**array([8,9,10,11,12,13,14,15,16,17,18]);
N_threads = array([1,2]);
L_FFTs = 2**array([8,9]);

X_f  = FastConvolution_float_float_int16_t_complex_f( l_kernel, l_data, l_fft, convolution_type , n_threads );
X_d  = FastConvolution_double_double_int16_t_complex_d( l_kernel, l_data, l_fft, convolution_type , n_threads );
for n_threads in N_threads:
    print "n_threads = {}".format(n_threads);
    X_f.n_threads=n_threads;
    X_d.n_threads=n_threads;
    for l_fft in L_FFTs:
        print "l_fft = {}".format(l_fft);
        X_f.l_fft = l_fft;
        X_d.l_fft = l_fft;
        ipython.magic ("time  X_f.prepare();");
        ipython.magic ("time  X_d.prepare();");
       
       
