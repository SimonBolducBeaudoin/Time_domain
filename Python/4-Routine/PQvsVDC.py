#!/bin/env/python
#! -*- coding: utf-8 -*-

"""
Simplpe script that 
    - aquires data from guzik
    - calculates p and q for many input filters
    - Accumulates histograms of ps and qs
    - Calculates culants
    - Displays expval of n and n^^2
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");

from my_python_library import *
from time_quadratures import *
from Histograms import *

## Experiment parameters
# Kernels params
n_kernels = 4 ;
l_kernel = (1<<8) + 1 ; 
# Acquisition params
dt = 0.03125 ;    
l_data= (1<<30 ) ;
f_max_analogue = 10.0 ;
f_min_analogue = 0.5 ;
# Filters params
n_filters = 3 ; 
l_hc = l_kernel//2 + 1 ;
df = 0.1 ;
f1 = DFT_freq( 8 , l_dft = l_kernel , dt = dt );
f2 = DFT_freq( 32 , l_dft = l_kernel , dt = dt );
f3 = DFT_freq( 64 , l_dft = l_kernel , dt = dt );
# Windowing params
alpha = 0.75 ;
# Algorithm params
l_fft = 1<<10;
n_threads = 2 ; # Warning the number of thread used might be changed by TimeQuad class and Histogram class
# Guzik params
gain_dB = 0 # 11.0 ; # Warning : choose this wisely before running
# DC voltages
Vdc = linspace( 0 , 5 , 10 ) ; 
#######################################################################################################
## Building filters
    # f \approx 1 , 4 and 8 (calculated using DFT_freq)   
print "\n Filter list :"
Filters = empty( (n_filters,l_hc) , dtype=complex , order='C' ) ;
Filters[0,:] = Gaussian_filter( f1 , df=df , l_kernel = l_kernel , dt = dt ) ;
Filters[1,:] = Gaussian_filter( f2 , df=df , l_kernel = l_kernel , dt = dt ) ;
Filters[2,:] = Gaussian_filter( f3 , df=df , l_kernel = l_kernel , dt = dt ) ;

print "\n TimeQuad object :"
X  = TimeQuad(l_kernel=l_kernel , n_kernels=n_kernels , l_data=l_data , dt=dt , f_max_analogue=f_max_analogue , f_min_analogue=f_min_analogue ,  filters=Filters , alpha=alpha , l_fft=l_fft , n_threads=n_threads);
ks_p = X.ks_p();
ks_q = X.ks_q();
ps  = X.ps();
qs  = X.qs();

print "\n Histograms :"
max = 10.0*10**5; # first estimate the width of p and q distributions
nb_of_bin = 2**10 ;

Hs_shape = (len(Vdc) , n_kernels ) ;

Hs = r_[[Histogram2D_uint64_t(nb_of_bin,n_threads) for i in range(prod(Hs_shape))]] ;
Hs.shape = Hs_shape ;
H_x = Hs[0,0].abscisse(max) ;
 
print "\n Guzik :"
gz = load_guzik() ;
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=True, ext_ref='int')

print "\n Yoko :"
# A verifier 
load("yo10")
set(yo10,0)
set(yo10.output_en, True)
set(yo10.range,10)

#######################################################################################################
## Makes measurments 

number_of_measures = 5 ;
t_begin = time.time(); 
for j , vdc in enumerate( Vdc ):

    # Call function that sets proper voltages and waits
    print ("\n\nMeasure {:03}/{:03d}").format(i+1, number_of_measures)
    print ("   n_threads : {:02}/{:02d}").format(n_threads,72)
    
    t0 = time.time(); 
    data = get( gz );# int16
    t1 = time.time();
    
    X.execute( data );
    t2 = time.time();
    
    for i in range(n_kernels) :
        Hs[j,i].accumulate( ps[i,:] , qs[i,:] , max = max ) ;

    t3 = time.time(); 
    
    print "   Data Acquisition:  {:04.2F}  [s] |  Quadrature calculations :  {:04.2F}  [s] |  Histogram accumulation: {:.2F}  [s] ".format( t1-t0, t2-t1, t3-t2)
t_end = time.time(); 

Estimated_time_per_loop = 4*5.0 ;
print("Experience over : \n \t Estimated time [s] {:04.2F} \n \t Real time {:04.2F} [s] \n ").format( number_of_measures*Estimated_time_per_loop , t_end-t_begin )
print("Process rate : {:04.2F} [GSa/s] ").format(10**(-9)*l_data*number_of_measures/(t_end-t_begin))

#######################################################################################################
#### DISPLAY RESULTS
height = 8;

# Quadratures
# fig , axs = subplots( 4,1, sharex = True)
# fig.set_size_inches( 1.62*height , height )
# fig.subplots_adjust(hspace = 0.5 , wspace = 0)

# t_kernel    = (arange(l_kernel) - l_kernel//2)*dt ;
# t_data      = (arange(l_data)   - l_data//2)  *dt ;
# t_valid      = (arange(p.size)   - p.size//2)  *dt ;

# axs[0].plot(t_kernel, kernel_p , t_kernel , kernel_q)
# axs[0].set(title = "Kernels")
# axs[1].plot(t_data , data )
# axs[1].set(title = "Data")
# axs[2].plot(t_valid , p , t_valid , q)
# axs[2].set(title = "Quadratures")
# axs[3].plot(t_valid , sqrt( p**2 + q**2 ) )
# axs[3].set(title = "Photon flux ")
# axs[3].set_xlabel(" t [ns]")

# histogram 1D

# fig , axs = subplots( 2,1, sharex = True)

# fig.set_size_inches( 1.62*height , height )
# fig.subplots_adjust(hspace = 0.5 , wspace = 0)

# axs[0].plot( H_abscisse, H_p.get() )
# axs[0].set(title = "p")
# axs[1].plot( H_abscisse , H_q.get() )
# axs[1].set(title = "q")

# histogram 2D

# figure();
# imshow(H2D.get(),cmap='Reds', interpolation='none' , extent=[H2D_abscisse[0],H2D_abscisse[-1],H2D_abscisse[0],H2D_abscisse[-1]] );
# colorbar();
