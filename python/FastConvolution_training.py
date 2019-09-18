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
## <Output_Type , Kernel_Type , DataType > = < int16_t , int16_t , int16_t >

X  = FastConvolution_float_float_int16_t_complex_f( l_kernel, l_data, l_fft, convolution_type , n_threads );
print
X.prepare();
X  = FastConvolution_double_double_int16_t_complex_d( l_kernel, l_data, l_fft, convolution_type , n_threads );
X.prepare();

del X, data, kernel, output_full, output_valid