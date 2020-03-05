#!/bin/python
# -*- coding: utf-8 -*-

"""
    - Test with many data aquisition and 2 gaussian filters and declaring 3 kernels pair
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py")

from time_quadratures import *

## Setup variables
l_kernel = (1<<8) + 1 ;
n_kernels = 3 ;
l_data= (1<<14) ;
dt = 0.03125 ;
f_max_analogue = 10.0 ;
f_min_analogue = 0.5 ;
alpha = 0.25 ;
l_fft = 1<<10;
n_threads = 36 ;

l_hc = l_kernel//2 + 1 ;

Filters = empty( (2,l_hc) , dtype=complex , order='C' ) ;
Filters[0,:] = Gaussian_filter( f=1.0 , df = 0.1 , l_kernel = l_kernel , dt = dt ) ;
Filters[1,:] = Gaussian_filter( f=5.0 , df = 0.1 , l_kernel = l_kernel , dt = dt ) ;

X  = TimeQuad(l_kernel=l_kernel , n_kernels=n_kernels , l_data=l_data , dt=dt , f_max_analogue=f_max_analogue , f_min_analogue=f_min_analogue ,  filters=Filters , alpha=alpha , l_fft=l_fft , n_threads=n_threads);

ks_p = X.ks_p();
ks_q = X.ks_q();
ps  = X.ps();
qs  = X.qs();

mu=0
sigma=2**12

n_repetition = 2 ;
for i in range(n_repetition):
    data  = int16( normal(mu,sigma,l_data) );

    X.execute( data );

    # fig, axs = subplots(2,1)
    # axs[0].plot( ks_p[0,:] )
    # axs[0].plot( ks_q[0,:] )
    # axs[1].plot( ks_p[1,:] )
    # axs[1].plot( ks_q[1,:] )

    fig, axs = subplots(2,1)
    axs[0].plot( ps[0,:] )
    # axs[0].set_ylim(-10**6,10**6)
    axs[1].plot( qs[0,:] )
    
    print" i = {}, ps:m_2 = {}, qs:m_2 = {}".format(i , scipy.stats.moment(ps[0,:],2), scipy.stats.moment(qs[0,:],2))

    fig, axs = subplots(2,1)
    axs[0].plot( ps[1,:] )
    # axs[0].set_ylim(-10**6,10**6)
    axs[1].plot( qs[1,:] )
    
    fig, axs = subplots(2,1)
    axs[0].plot( ps[2,:] )
    # axs[0].set_ylim(-10**6,10**6)
    axs[1].plot( qs[2,:] )

