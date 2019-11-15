#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
# from Time_quadratures import *

####################################
# Measurement
dt = 0.03125; # [ns]; # Default value

## Odd Kernel Construction
data , t = WhiteNoise( duree = 10**5 ) ; # duree [ns]
duree_kernel = ( (1<<10) + 1 ) * dt ;
l_kernel = int ( duree_kernel//dt ) ;
if l_kernel%2 != 1 : l_kernel += 1
(NoyauP,NoyauQ) = Kernels_impaires(l_kernel, dt )   # [ns^-1/2]

figure(1)
plot(NoyauP, '-+')
plot(NoyauQ, '-+')
legend()

figure(2)
plot(fftfreq(NoyauP.size)*(1/dt),fft(NoyauP), '-+')
plot(fftfreq(NoyauQ.size)*(1/dt),fft(NoyauQ), '-+')
legend()