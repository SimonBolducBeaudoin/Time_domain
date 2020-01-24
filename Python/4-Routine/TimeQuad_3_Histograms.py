#!/bin/env/python
#! -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");
from time_quadratures import *
from Histograms import *

## Experiment parameters
    # TimeQuad params
l_kernel = (1<<8) + 1 ;
l_data= 1<<30;
dt = 0.03125 ;
    # Guzik params
gain_dB = 11.0 ; # Warning : choose this wisely before running
    # Computing parameter
n_threads = 36 ;

#######################################################################################################

## Instanciate TimeQuad object
print "\nInstanciate TimeQuad object :"
TimeQuads  = TimeQuad_int16_t( l_kernel = l_kernel, l_data = l_data , dt=dt , n_threads=n_threads );
    # Passing memory to numpy variables
data = array(TimeQuads.data, copy = False);
kernel_p = array(TimeQuads.kernel_p, copy = False);
kernel_q = array(TimeQuads.kernel_q, copy = False);
p = array(TimeQuads.p, copy = False);
q = array(TimeQuads.q, copy = False);

## Loading GUZIK
print "\Loading GUZIK :"
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()
    
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=True, ext_ref='int')

## Initializing histogram
print "\nInstanciate histograms :"
# You'll need an estimate of the width of your p and q distribution
max = 10.0*10**5;
nb_of_bin = 2**12

    # Histogram 1D
# H_p = Histogram_uint64_t( nb_of_bin , n_threads);
# H_q = Histogram_uint64_t( nb_of_bin , n_threads);   
# H_abscisse = H_p.abscisse( max ); # Histograms.abscisse( DataAvatar , max );

    # Histogram 2D   
H2D = Histogram2D_uint64_t( nb_of_bin , n_threads);

H2D_abscisse = H2D.abscisse( max ); # Histograms.abscisse( DataAvatar , max );

#######################################################################################################
## Iterative measurments 
    

number_of_measures = 5 ;
t_begin = time.time(); 
for i_value, i in enumerate( range( number_of_measures ) ):
    
    print ("\n\nMeasure {:03}/{:03d}").format(i+1, number_of_measures)
    print ("   n_threads : {:02}/{:02d}").format(n_threads,72)
    # 1 - Data Acquisition!
    t0 = time.time(); 
    data[:] = get( gz );# int16
    t1 = time.time();
    
    # 2 - Quadrature calculations 
    TimeQuads.execute();
    t2 = time.time();
    
    # 4 - Histogram accumulation 
    # H_p.accumulate(p,max);
    # H_q.accumulate(q,max);
    H2D.accumulate(p,q,max);
    t3 = time.time(); 
    
    print "   Data Acquisition:  {:04.2F}  [s] |  Quadrature calculations :  {:04.2F}  [s] |  Histogram accumulation: {:.2F}  [s] ".format( t1-t0, t2-t1, t3-t2)
t_end = time.time(); 

Estimated_time_per_loop = 5.0 ;
Clipping = H2D.get()[0,:].sum() + H2D.get()[-1,:].sum() +  H2D.get()[:,0].sum() +  H2D.get()[:,-1].sum() - ( H2D.get()[0,0].sum() + H2D.get()[-1,-1].sum() +  H2D.get()[-1,0].sum() +  H2D.get()[0,-1].sum() )
print("Experience over : \n \t Estimated time [s] {:04.2F} \n \t Real time {:04.2F} [s] \n \t total clipping = {:d}/{:d} = {:F} ").format( number_of_measures*Estimated_time_per_loop , t_end-t_begin, Clipping, H2D.get().sum() , float(Clipping)/H2D.get().sum() )
print("Effective treatement speed : {:04.2F} [GSa/s] ").format(10**(-9)*l_data*number_of_measures/(t_end-t_begin))

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

fig , axs = subplots( 2,1, sharex = True)

fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)

axs[0].plot( H_abscisse, H_p.get() )
axs[0].set(title = "p")
axs[1].plot( H_abscisse , H_q.get() )
axs[1].set(title = "q")

# histogram 2D

figure();
imshow(H2D.get(),cmap='Reds', interpolation='none' , extent=[H2D_abscisse[0],H2D_abscisse[-1],H2D_abscisse[0],H2D_abscisse[-1]] );
colorbar();
