#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
# from Time_quadratures import *

####################################
# Measurement
dt = 0.03125; # [ns]; # Default value
# data , t = NoisySinSquare( dt = dt  ) ;
data , t = NoisySinGauss( f = 1 , SNR = 10000, number_of_cycle_width =  10 ) ;
# l_data = data.size;


####################################
## Odd Kernel Construction
l_kernel = (1<<14) + 1 ;
# if l_kernel<= l_data : ValueError("l_kernel has to be larger than l_data \n")
####################################

####################################
# Convolution output parmams
# l_full = l_kernel + l_data - 1 ;
# l_valid = l_data - l_kernel + 1 ;
# t_valid = arange( l_valid )*dt ;
# t_kernel = arange (l_kernel) * dt ;
####################################

#### Using scipy and python library
print "Calculation with Scipy"
# print "\t l_kernel = {}".format(l_kernel)
# print "\t l_data = {}".format(l_data)
(NoyauP,NoyauQ) = Kernels_impaires(l_kernel, dt)   # [ns^-1/2]
p = fftconvolve(NoyauP,data,mode='valid') # shape 9001
q = fftconvolve(NoyauQ,data,mode='valid')# 
####

A_p = p.max() ;
A_q = q.max() ;
print "p.max() = {} ".format(p.max())
print "q.max() = {} ".format(q.max())
print "A_p/A_q() = {} ".format(A_p/A_q)

# figure(1)
# plot(NoyauP, '-+')
# plot(NoyauQ, '-+')
# legend()

figure(2)
plot( t , data )

t_valid = (arange(p.size) - p.size//2)* dt ;

figure(3)
plot( t_valid , p , '-' ,label = "p" )
plot( t_valid , q , '-' , label = "q")
plot(  t_valid , zeros(p.size)  )
legend();

figure(4)
plot( t_valid , sqrt( p**2 + q**2 ) )
legend()
