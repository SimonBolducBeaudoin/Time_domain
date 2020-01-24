#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
# from Time_quadratures import *

####################################
# Measurement
dt = 0.03125; # [ns]; # Default value

## Odd Kernel Construction
data , t = WhiteNoise( duree = 10**3 ) ; # duree [ns]
duree_kernel = ( (1<<8) + 1 ) * dt ;
l_kernel = int ( duree_kernel//dt ) ;
if l_kernel%2 != 1 : l_kernel += 1
(NoyauP,NoyauQ) = Kernels(l_kernel, dt )   # [ns^-1/2]
p = fftconvolve(NoyauP,data,mode='valid') # shape 9001
q = fftconvolve(NoyauQ,data,mode='valid')# 

figure(1)
plot(NoyauP, '-+')
plot(NoyauQ, '-+')
legend()

# figure(2)
# plot( t , data )

t_valid = (arange(p.size) - p.size//2)* dt ;

figure(3)
plot( t_valid , p  , '-' , label = "p" )
plot( t_valid , q , '-' , label = "q")
plot(  t_valid , zeros(p.size)  )
legend();

figure(4)
plot( t_valid  , sqrt( p**2 + q**2 )  )
legend()
