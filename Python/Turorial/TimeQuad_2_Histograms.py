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
l_data= 1<<24;
dt = 0.03125 ;
    # Guzik params
gain_dB = 11.0 ; # Warning : choose this wisely before running

## Instanciate TimeQuad object
print "\nInstanciate TimeQuad object :"
TimeQuads  = TimeQuad_int16_t( l_kernel = l_kernel, l_data = l_data , dt=dt , n_threads = 36 );
    # Passing memory to numpy variables
data = array(TimeQuads.data, copy = False);
kernel_p = array(TimeQuads.kernel_p, copy = False);
kernel_q = array(TimeQuads.kernel_q, copy = False);
p = array(TimeQuads.p, copy = False);
q = array(TimeQuads.q, copy = False);

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
# You'll need an estimate of the width of your p and q distribution
max = 6.0*10**5;

    # Histogram 1D
H_p = Histogram_uint64_t(2**16 , 36);
H_q = Histogram_uint64_t(2**16 , 36);

    # Histogram 2D
H2D = Histogram2D_uint64_t(2**12 , 36);

H_abscisse = H_p.abscisse( max ); # Histograms.abscisse( DataAvatar , max );

## Single measurment and histogram
    # Fetching data from Guzik
data[:] = get( gz );# int16
    # Calculating quadratures (Quadratures results are now in p and q)
TimeQuads.execute();
H_p.accumulate(p,max);
H_q.accumulate(q,max);

H2D.accumulate(p,q,max);

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
imshow(H2D.get(),cmap='Reds', interpolation='none' , extent=[H_abscisse[0],H_abscisse[-1],H_abscisse[0],H_abscisse[-1]] );
colorbar();
