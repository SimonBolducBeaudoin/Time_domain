#!/bin/python
# -*- coding: utf-8 -*-

execfile("headers.py")

L_kernel = 1<<8;
L_data= 1<<20;
L_FFT = 1<<10;

dt = 0.03125; # [ns]; # Default value
Convolution_type = 'fft'; # Default value

############# Declare Time_Quad objects
X = Time_Quad_double_int16_t( L_kernel, L_data, L_FFT);

# Numpy instance of arrays
data = array(X.data, copy = False);

kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);

p_full = array(X.p_full, copy = False);
p_valid = array(X.p_valid, copy = False);

q_full = array(X.q_full, copy = False);
q_valid = array(X.q_valid, copy = False);
#############

# Generate random data
mu=0
sigma=2**12
data[:]  = int16( normal(mu,sigma,L_data) );

#### Using scipy and python library
(NoyauP,NoyauQ) = Kernels_time(L_kernel, dt)   # [ns^-1/2]
P_i_scipy = fftconvolve(NoyauP,data,mode='full') # shape 9001
Q_i_scipy = fftconvolve(NoyauQ,data,mode='full')
####

#### Using Time_Quad
X.Convolution_type = 'fft';
X.execute();
####

#### Diplay results

fig, axs = subplots(3,1)
SLICE = slice( None, None, None );
axs[0].plot(P_i_scipy[SLICE])#;axs[0,1].plot(Q_i_scipy);
axs[1].plot(p_full[SLICE])#;axs[1,1].plot(q_full);
axs[2].plot( (P_i_scipy[SLICE]-p_full[SLICE]) )
#axs[1,0].hist(P_i_scipy,100);axs[1,1].hist(Q_i_scipy,100);

####