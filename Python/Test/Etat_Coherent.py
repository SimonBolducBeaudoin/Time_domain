#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

dt = 0.03125; # [ns]; # Default value
Nyquist = 1/(2*dt) ;

l_kernel_q = (1<<10) + 1 ;
l_data = 2*l_kernel_q
l_valid_q = l_data - l_kernel_q + 1 ;
frequences = fftfreq( l_valid_q ,dt );

f = frequences[25];

data , t = SinCycle( f , n=l_data ) ;
(NoyauP,NoyauQ) = Kernels(l_kernel_q, dt)   # [ns^-1/2]
p = fftconvolve(NoyauP,data,mode='valid') # shape 9001
q = fftconvolve(NoyauQ,data,mode='valid')# 
    
t_valid = (arange(p.size) - p.size//2)* dt ;

figure()
plot( t_valid , p , '-' ,label = "p" )
plot( t_valid , (1/sqrt(f))*data[:t_valid.size] )
plot( t_valid , q , '-' , label = "q")    
plot(  t_valid , zeros(p.size)  )
legend;

figure()
plot( t_valid , sqrt( p**2 + q**2 ) )
legend()
