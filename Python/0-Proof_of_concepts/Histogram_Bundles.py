#!/bin/env/python
#! -*- coding: utf-8 -*-

"""
How to organize histogram objects into bundles of objects using numpy
    - Intanciate an array of object with list comprehension
    - Call accumulate of each individual histogram
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");

from Histograms import *

nb_of_bin = 2**10 ;

n_j = 3 ;
n_i = 2 ;

Hs_shape = ( n_j , n_i ) ;
Hs = r_[[Histogram2D_uint64_t(nb_of_bin,2) for i in range(prod(Hs_shape))]] ;
Hs.shape = Hs_shape ;
 
max = 2.0**9 ; # En attendant de correctement implémenter les histograms de int16
H_x = Hs[0,0].abscisse(max) ;


l_data = 2**20
mu=0
sigma=2**6
data_1  = ( normal(mu,sigma,l_data) );
data_2  = ( normal(mu,sigma,l_data) );

for j in range(n_j) :
    for i in range(n_i) :
        Hs[j,i].accumulate( data_1 , data_2, max = max ) ;
        
        
figure();
imshow(Hs[0,0].get(),cmap='Reds', interpolation='none' , extent=[H_x[0],H_x[-1],H_x[0],H_x[-1]] );
colorbar();

figure();
imshow(Hs[0,1].get(),cmap='Reds', interpolation='none' , extent=[H_x[0],H_x[-1],H_x[0],H_x[-1]] );
colorbar();
