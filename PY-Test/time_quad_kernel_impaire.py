#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from Time_quadratures import *

####################################
## Setup variables
l_kernel = (1<<8) + 1 ;
l_data= 1<<20;
l_full = l_kernel + l_data - 1 ;
l_valid = l_data - l_kernel + 1 ;

mu=0
sigma=2**13
####################################

# Generate random data

dt = 1.0; # [ns]; # Default value
f = 1.0/(l_kernel-1);
# nb d'ech dans un cycle
n = int(1/(dt*f));
omega = 2*pi*f;
t = arange(l_data)*dt ;
t_valid = arange( l_valid )*dt ;
t_kernel = arange (l_kernel) * dt ;
data = ( (1<<14)*sin(omega*t) + normal(mu,sigma,l_data) );
# data =  sin(omega*t)  ;

def Kernels_impaires(l_kernel,dt) :
    # Returns the time kernel of p(t) and q(t)
    # Kernel de p(t) => 1/sqrt(|t|) 
    # Kernel de q(t) => sgn(t)/sqrt(|t|)
    
    half_abscisse = dt*( arange(l_kernel//2) + 1 ) ;
    
    half_kernel  = 1/sqrt(absolute(half_abscisse)) ;
    
    # kernel_p = concatenate( (half_kernel[::-1] , [(-2)*(half_kernel.sum())] , half_kernel ) ) ;
    kernel_p = concatenate( (half_kernel[::-1] , [0] , half_kernel ) ) ;
    kernel_q = concatenate( ((-1)*half_kernel[::-1] , [0] , half_kernel ) ) ;

    # normalisation
    # Energy_p = ( (absolute(kernel_p)).sum() ) ;
    # Energy_q = ( (absolute(kernel_q)).sum() ) ;
    # kernel_p = Energy_q*kernel_p/Energy_p ;
    # kernel_q = kernel_q/(absolute(kernel_q).sum())
    return kernel_p, kernel_q

#### Using scipy and python library
print "Calculation with Scipy"
(NoyauP,NoyauQ) = Kernels_impaires(l_kernel, dt)   # [ns^-1/2]
# p_full = convolve(NoyauP,data,mode='full') # shape 9001
# q_full = convolve(NoyauQ,data,mode='full')
p_full = fftconvolve(NoyauP,data,mode='full') # shape 9001
q_full = fftconvolve(NoyauQ,data,mode='full')
####
SLICE = slice( l_kernel-1 , l_data , None )
p_valid = p_full[SLICE];
q_valid = q_full[SLICE];


print "p_valid.sum() = {} ".format(p_valid[:n].sum())
print "q_valid.sum() = {} ".format(q_valid[:n].sum())
A_p = p_valid.max() ;
A_q = q_valid.max() ;
print "p_valid.max() = {} ".format(p_valid.max())
print "q_valid.max() = {} ".format(q_valid.max())
print "A_p/A_q() = {} ".format(A_p/A_q)

figure(1)
plot(NoyauP, '-+')
plot(NoyauQ, '-+')
# plot(data[:l_kernel])
legend()

figure(2)
plot( t_valid[:n] ,  p_valid[:n] , '-' ,label = "p" )
plot( t_valid[:n] ,  q_valid[:n] , '-' , label = "q")
plot( t_valid[:n] ,  zeros(t_valid[:n].size)  )
legend();

figure(4)
plot( t_valid ,sqrt( p_valid**2 + q_valid**2 ) )
legend()

figure(5)
plot( t_valid , p_valid + q_valid  )
legend()