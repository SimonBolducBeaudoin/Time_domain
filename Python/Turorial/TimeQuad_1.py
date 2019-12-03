#!/bin/env/python
#! -*- coding: utf-8 -*-

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");
from time_quadratures import *

## Experiment parameters
    # TimeQuad params
l_kernel = (1<<8) + 1 ;
l_data= 1<<14;
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
p_full = array(TimeQuads.p_full, copy = False);
q_full = array(TimeQuads.q_full, copy = False);

## Loading GUZIK
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()
    
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=True, ext_ref='int')

## The EXPERIMENT
    # Fetching data from Guzik
data[:] = get( gz );
    # Calculating quadratures (Quadratures results are now in p_full and q_full)
TimeQuads.execute();

#### DISPLAY RESULTS

fig , axs = subplots( 4,1, sharex = True)
fig.subplots_adjust(hspace = 0.5 , wspace = 0)

t_kernel    = (arange(l_kernel) - l_kernel//2)*dt ;
t_data      = (arange(l_data)   - l_data//2)  *dt ;
t_full      = (arange(p_full.size)   - p_full.size//2)  *dt ;

axs[0].plot(t_kernel, kernel_p , t_kernel , kernel_q)
axs[0].set(title = "Kernels")
axs[1].plot(t_data , data )
axs[1].set(title = "Data")
axs[2].plot(t_full , p_full , t_full , q_full)
axs[2].set(title = "Quadratures")
axs[3].plot(t_full , sqrt( p_full**2 + q_full**2 ) )
axs[3].set(title = "Photon flux ")
axs[3].set_xlabel(" t [ns]")

