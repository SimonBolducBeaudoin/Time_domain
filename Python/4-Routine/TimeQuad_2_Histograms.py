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
n_kernels = 1 ;
l_kernel = (1<<8) + 1 ;
l_data= 1<<28;
dt = 0.03125 ;
# Windowing params
alpha = 0.75 ;
# Algorithm params
l_fft = 1<<10;
n_threads = 36 ; # Warning the number of thread used might be changed by TimeQuad class and Histogram class
    # Guzik params
gain_dB = 11.0 ; # Warning : choose this wisely before running

## Instanciate TimeQuad object
print "\nInstanciate TimeQuad object :"
X = TimeQuad(l_kernel=l_kernel , n_kernels=n_kernels , l_data=l_data , dt=dt , alpha=alpha , l_fft=l_fft , n_threads=n_threads);
    # Passing memory to numpy variables
ks_p = X.ks_p();
ks_q = X.ks_q();
ps  = X.ps();
qs  = X.qs();

## Loading GUZIK
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()
    
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=True, ext_ref='int')

## Initializing histogram
# You'll need an estimate of the width of your ps[0,:] and qs[0,:] distribution
max = 4.5*10**5;

    # Histogram 1D
H_p = Histogram_uint64_t(2**16 , 36);
H_q = Histogram_uint64_t(2**16 , 36);

    # Histogram 2D
H2D = Histogram2D_uint64_t(2**12 , 36);

H_abscisse = H_p.abscisse( max ); # Histograms.abscisse( DataAvatar , max );

## Single measurment and histogram
    # Fetching data from Guzik
data = get( gz );# int16
    # Calculating quadratures (Quadratures results are now in ps[0,:] and qs[0,:])
X.execute(data);
H_p.accumulate(ps[0,:],max);
H_q.accumulate(qs[0,:],max);

H2D.accumulate(ps[0,:],qs[0,:],max);

#### DISPLAY RESULTS
height = 8;

# Quadratures
# fig , axs = subplots( 4,1, sharex = True)
# fig.set_size_inches( 1.62*height , height )
# fig.subplots_adjust(hspace = 0.5 , wspace = 0)

# t_kernel    = (arange(l_kernel) - l_kernel//2)*dt ;
# t_data      = (arange(l_data)   - l_data//2)  *dt ;
# t_valid      = (arange(ps[0,:].size)   - ps[0,:].size//2)  *dt ;

# axs[0].plot(t_kernel, ks_p[0,:] , t_kernel , ks_q[0,:])
# axs[0].set(title = "Kernels")
# axs[1].plot(t_data , data )
# axs[1].set(title = "Data")
# axs[2].plot(t_valid , ps[0,:]  , t_valid , qs[0,:] )
# axs[2].set(title = "Quadratures")
# axs[3].plot(t_valid , sqrt( ps[0,:] **2 + qs[0,:]**2 ) )
# axs[3].set(title = "Photon flux ")
# axs[3].set_xlabel(" t [ns]")

# histogram 1D

fig , axs = subplots( 2,1, sharex = True)

fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)

axs[0].plot( H_abscisse, H_p.get() )
axs[0].set(title = "ps[0,:]")
axs[1].plot( H_abscisse , H_q.get() )
axs[1].set(title = "qs[0,:]")

# histogram 2D

figure();
imshow(H2D.get(),cmap='Reds', interpolation='none'  );
colorbar();
