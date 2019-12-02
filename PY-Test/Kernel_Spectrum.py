#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from time_quadratures import *

####################################
# Measurement
l_kernel = (1<<8) + 1 ;
l_data= 1<<14;
l_fft = 1<<10;
n_threads = 2 ;
dt = 0.03125; # [ns]; # Default value

alpha = 0.5 ;
(NoyauP,NoyauQ) = Kernels( l_kernel, dt , alpha )   # [ns^-1/2]
X  = TimeQuad_int16_t( l_kernel = l_kernel, l_data = l_data , l_fft = l_fft , dt=dt , convolution_type="fft" ,n_threads = n_threads );
kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);

# Window = Window_Tukey( 0.5, l_kernel ) ;
figure()
t_kernel = ( arange(l_kernel) - l_kernel//2 ) * dt; 
plot(t_kernel , NoyauP , '-+')
plot(t_kernel , kernel_p, '-+')
plot(t_kernel , NoyauQ , '-+')
plot(t_kernel , kernel_q, '-+')
plot(t_kernel , (1.0)/sqrt(absolute(t_kernel)) , '-')
# plot(t_kernel , Window_Tukey( alpha , l_kernel ) , '-')
xlabel("t[ns]");
legend()

figure()
frequences = fftfreq(NoyauP.size,dt) ; 
plot( frequences , absolute ( fft(NoyauP , norm="ortho") ), '-+' , label = "NoyauP window")
plot( frequences , absolute ( fft(kernel_p , norm="ortho") ), '-+' , label = "kernel_p window")
plot( frequences , absolute ( fft(NoyauQ , norm="ortho") ), '-+' , label = "NoyauQ window")
plot( frequences , absolute ( fft(kernel_q , norm="ortho") ), '-+' , label = "kernel_q window")
plot( frequences , 1.0/sqrt(absolute ( frequences ) ), '-', label = "analytique")
xlabel("f[GHz]");
xlim((-16,16))
legend()

# figure()
# frequences = fftfreq(NoyauP.size,dt) ; 
# plot( frequences , absolute ( fft(NoyauP, norm="ortho") ), '-+' , label = "kernel_p window")
# plot( frequences , absolute ( fft(NoyauQ, norm="ortho") ), '-+' , label = "kernel_q window")
# plot( frequences , 1.0/sqrt(absolute ( frequences ) ), '-', label = "analytique")
# xlabel("f[GHz]");
# xlim((-1,1))
# legend()

del X