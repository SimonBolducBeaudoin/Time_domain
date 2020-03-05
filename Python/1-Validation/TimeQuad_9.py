#!/bin/python
# -*- coding: utf-8 -*-

"""
    - Test with 2 data aquisition , n_filters = 1 , n_kernels = 2 and l_data =2**28
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py")

from time_quadratures import *

## Setup variables
l_kernel = (1<<8) + 1 ;
l_data= (1<<29) ;
l_fft = (1<<10) ;
n_kernels = 2 ;
dt = 0.03125 ;
n_threads = 36 ;

l_hc = l_kernel//2 + 1 ;

Filters = 0.01*ones( (n_kernels-1,l_hc) , dtype=complex , order='C' ) ;

X  = TimeQuad(l_kernel=l_kernel , n_kernels=n_kernels , l_data=l_data , dt=dt ,  filters=Filters , alpha=0.75 , n_threads=n_threads);

ks_p = X.ks_p();
ks_q = X.ks_q();
ps  = X.ps();
qs  = X.qs();

mu=0
sigma=2**12


data  = int16( normal(mu,sigma,l_data) );

X.execute( data );

# fig, axs = subplots(2,1)
# axs[0].plot( ks_p[0,:] )
# axs[0].plot( ks_q[0,:] )
# axs[1].plot( ks_p[1,:] )
# axs[1].plot( ks_q[1,:] )

fig, axs = subplots(2,1)
axs[0].plot( ps[0,:][:10000] )
# axs[0].set_ylim(-10**6,10**6)
axs[1].plot( qs[0,:][:10000]  )

fig, axs = subplots(2,1)
axs[0].plot( ps[1,:][:10000]  )
# axs[0].set_ylim(-10**6,10**6)
axs[1].plot( qs[1,:][:10000]  )

fig, axs = subplots(2,1)
axs[0].plot( ps[0,:][-10000:] )
# axs[0].set_ylim(-10**6,10**6)
axs[1].plot( qs[0,:][-10000:]  )

fig, axs = subplots(2,1)
axs[0].plot( ps[1,:][-10000:]  )
# axs[0].set_ylim(-10**6,10**6)
axs[1].plot( qs[1,:][-10000:]  )

