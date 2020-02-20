#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from time_quadratures import *

####################################
# Measurement
dt = 0.03125; # [ns]; # Default value
f = 5.0
SNR = 1000
data_tmp , t = NoisySinGauss( f = f , SNR = 1000, number_of_cycle_width =  10.0 ) ;
l_kernel = (1<<8) + 1 ;
alpha = 0.75
l_fft = 1<<10;
n_threads = 2 ;
dt = 0.03125 ;
####################################

X  = TimeQuad_int16_t( l_kernel = l_kernel, l_data = data.size , l_fft = l_fft , dt=dt , convolution_type="fft" ,n_threads = n_threads );
data = array(X.data, copy = False);
kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);
p_full = array(X.p_full, copy = False);
q_full = array(X.q_full, copy = False);
data[:] = data_tmp 
X.execute();

#### Using scipy and python library
# print "Calculation with Scipy"
# (NoyauP,NoyauQ) = Kernels(l_kernel, dt, alpha)   # [ns^-1/2]
# p = fftconvolve(NoyauP,data,mode='valid') # shape 9001
# q = fftconvolve(NoyauQ,data,mode='valid')# 
####

fig , axs = subplots( 4,1, sharex = True)
fig.subplots_adjust(hspace = 0.5 , wspace = 0)

f_string  = "$f$ = {} [GHz] :  $l_k$ = {} \n $alpha$ = {} : SNR = {} ".format(f, l_kernel, alpha, SNR)

fig.text(0.65, 0.89, f_string , fontsize=9)

# t_kernel    = (arange(l_kernel) - l_kernel//2)*dt ;
# t_valid     = (arange(p.size)   - p.size//2)  *dt ;

# axs[0].plot(t_kernel, NoyauP , t_kernel , NoyauQ)
axs[0].set(title = "Kernels")
# axs[1].plot(t , data )
axs[1].set(title = "Data")
# axs[2].plot(t_valid , p , t_valid , q)
axs[2].set(title = "Quadratures")
# axs[3].plot(t_valid , sqrt( p**2 + q**2 ) )
axs[3].plot( sqrt( p_full**2 + q_full**2 ) )
axs[3].set(title = "Photon flux ")
axs[3].set_xlabel(" t [ns]")


# figure(1)
# plot(NoyauP, '-+')
# plot(NoyauQ, '-+')
# legend

# figure(2)
# plot( t , data )


# figure(3)
# plot( t_valid , p , '-' , label = "p" )
# plot( t_valid , q , '-' , label = "q")
# legend();

# figure(4)
# plot( t_valid , sqrt( p**2 + q**2 ) )
# legend()
