#!/bin/python
# -*- coding: utf-8 -*-

"""
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py")

from bispectrum import *

## Setup variables
l_data= (1<<20) ;
n_threads = 2 ;
l_fft = 1<<10;


mu=0
data  = normal(size=l_data) ;
data=where(data<-1,0,data)

bispectre = bispectrum(data-mean(data),l_fft,n_threads)

figure();
imshow(absolute(bispectre),cmap='Reds', interpolation=None );
colorbar();
