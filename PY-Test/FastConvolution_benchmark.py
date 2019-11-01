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


# N_threads = array([1,2,4,8,16,32,64,72]);
# L_FFTs = 2**array([8,9,10,11,12,13,14,15,16,17,18]);
N_threads = array([2]);
L_FFTs = 2**array([9,10]); 

# Executions_f = zeros( (size(N_threads) , size(L_FFTs)) );
# Executions_d = zeros( (size(N_threads) , size(L_FFTs)) );

X_f  = FastConvolution_float_float_int16_t_complex_f( l_kernel, l_data, l_fft, convolution_type , n_threads );
# X_d  = FastConvolution_double_double_int16_t_complex_d( l_kernel, l_data, l_fft, convolution_type , n_threads );

# X_f.time_execution = 0 ;
# X_d.time_execution = 0 ;

for i , n_threads in enumerate(N_threads):
    print "n_threads = {}".format(n_threads);
    X_f.n_threads=n_threads;
    # X_d.n_threads=n_threads;
    for j, l_fft in enumerate(L_FFTs):
        print "l_fft = {}".format(l_fft);
        X_f.l_fft = l_fft;
        # X_d.l_fft = l_fft;
        print "execution "
        ipython.magic ("time  X_f.execute();");
        # ipython.magic ("time  X_d.execute();");
        
        # Executions_f[i,j] = X_f.time_execution ; 
        # Executions_d[i,j] = X_d.time_execution ;
        
        # X_f.time_execution = 0 ;
        # X_d.time_execution = 0 ;
      
# filename = "FastConvolution_PATIENT_train_alloc_execute.npz" 
# savez(filename, l_data = l_data ,n_threads = n_threads , l_fft = L_FFTs , 
# Allocations_f = Allocations_f , Allocations_d = Allocations_d , 
# Preparations_f = Preparations_f , Preparations_d = Preparations_d , 
# Executions_f = Executions_f , Executions_d = Executions_d ) ;

del X_f
# del X_d
