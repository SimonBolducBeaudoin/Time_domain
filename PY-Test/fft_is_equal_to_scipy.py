#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# 2019-10-31 Benchmarks doesn't output the result properly into inplace_halfC
# didn't bother to fix it now.

# Dont forget to drop your library.pyd in the current folder
from fft_benchmark import *
l_data= 1<<8;
l_fft = 1<<8;

mu = 0 ; 
sigma = 2**12 ;
n_threads = 1 ; 

# Numpy instance of arrays
X = FFT_double_int16_t(l_data , l_fft, n_threads );
data = array(X.data, copy = False);
inplace_halfC = array(X.inplace_halfC, copy = False);
data[:] = normal(mu,sigma, l_data);
#############

#### Using numpy
fft_numpy = rfft(data);
####

#### Using C++ 
X.execute();
####

Answer_1 = array_equal( fft_numpy , inplace_halfC );
# Answer_2 = allclose( fft_numpy , inplace_halfC , atol = 1e0 ) ;

print "Are equal = {} ".format( Answer_1  );

#### Diplay results

# fig, axs = subplots(3,1)
# SLICE = slice( None, None, None );
# axs[0].plot(P_i_scipy[SLICE])#;axs[0,1].plot(Q_i_scipy);
# axs[1].plot(p_full[SLICE])#;axs[1,1].plot(q_full);
# axs[2].plot( (P_i_scipy[SLICE]-p_full[SLICE]) )
#axs[1,0].hist(P_i_scipy,100);axs[1,1].hist(Q_i_scipy,100);

####