#!/bin/python
# -*- coding: utf-8 -*-

execfile("headers.py")

L_kernel = 1<<8;
L_data= 1<<12;
L_FFT = 1<<10;
n_threads = 4;

Convolution_type = 'fft'; # Default value

############# Declare Time_Quad objects
X = Fast_Conv_int16_t_int16_t_int16_t_complex_d( L_kernel, L_data, L_FFT, n_threads );

# Numpy instance of arrays
data = array(X.data, copy = False);
kernel = array(X.kernel, copy = False);
output_full = array(X.output_full, copy = False);
output_valid = array(X.output_valid, copy = False);
#############

# Generate random data
mu=0
sigma=2**8
data[:]  = int16( normal(mu,sigma,L_data) );
kernel[:]  = int16(numpy.ones(L_kernel, dtype='double'));

Scipy = fftconvolve(kernel,data,mode='full') # shape 9001
####

#### Using Time_Quad
X.Convolution_type = 'fft';
X.execute();
####

#### Diplay results

fig, axs = subplots(3,1)
SLICE = slice( None, None, None );
axs[0].plot(Scipy[SLICE])#;axs[0,1].plot(Q_i_scipy);
axs[1].plot(output_full[SLICE])#;axs[1,1].plot(q_full);
axs[2].plot( (Scipy[SLICE] - output_full[SLICE] ) )
#axs[1,0].hist(P_i_scipy,100);axs[1,1].hist(Q_i_scipy,100);

####

del X, data, kernel, output_full, output_valid