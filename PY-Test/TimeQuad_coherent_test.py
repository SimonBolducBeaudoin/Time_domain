#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from Time_quadratures import *

####################################
## Setup variables
l_kernel = (1<<8)+1; # doit être impaire
l_data= 1<<14;
l_fft = 1<<10;
l_full = l_kernel + l_data - 1 ;
l_valid = l_data - l_kernel + 1 ;
n_threads = 70 ;
convolution_type = 'fft'; # Default value
####################################

####################################
print "Instance de TimeQuad"
X  = TimeQuad_double_int16_t( l_kernel=l_kernel, l_data=l_data, l_fft=l_fft, convolution_type=convolution_type , n_threads=n_threads );

# Numpy instance of arrays (shared memory)
print "Passer la mémoire à des objets nump"
data = array(X.data, copy = False);
kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);
p_full = array(X.p_full, copy = False);
q_full = array(X.q_full, copy = False);

# Generate random data
mu=0
sigma=1<<8

dt = 0.03125; # [ns]; # Default value
f = 1;
omega = 2*pi*f;
t = arange(l_data)*dt ;
t_valid = arange( l_valid )*dt ;
t_kernel = arange (l_kernel) * dt ;
# data[:] =  int16( (1<<14)*sin(omega*t) + normal(mu,sigma,l_data) );

print "Asigner des valeurs à la variable data"
data[:] =  int16( (1<<14)*sin(omega*t)  );

print "Executer la convolution " 
X.execute();


# ##################################
# # fig, axs = subplots(3,1)
SLICE = slice( None, None, None );
# # axs[0].plot(q_full);
# # axs[1].plot(p_full);
# # axs[2].plot( sqrt (p_full**2 + q_full**2) );

figure(1)
plot( t_kernel,  kernel_p , '-*', label = "kernel_p")
plot( t_kernel,  kernel_q , '-*', label = "kernel_q")
legend();

figure(2)
plot( t_valid[:257] ,  p_full[SLICE][:257] , '-*', label = "p")
plot( t_valid[:257] ,  q_full[SLICE][:257] , '-*',label = "q")
legend();

figure(3)
SLICE = slice( l_kernel-1 , l_data , None )
plot( t_valid , p_full[SLICE] + q_full[SLICE] , '-*', label = "p(t) + q(t) "  )
legend();
# ylim( 0 , 500*10**3  )

figure(4)
SLICE = slice( l_kernel-1 , l_data , None )
plot( t_valid ,sqrt( p_full[SLICE]**2 + q_full[SLICE]**2 ) , '-*', label = "p**2 + q**2 "  )
legend();
# ylim( 0 , 500*10**3  )

del X