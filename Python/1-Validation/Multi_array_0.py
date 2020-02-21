#!/bin/python
# -*- coding: utf-8 -*-

"""
Validates that 
    - TimeQuad Object can be construsted
    - Pointers can be passed properly in between C++ and python
    - Outputs looks ok
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py")

from Multi_array import *

X = int8(r_[0:10:1]) ;
Y = Multi_array_int8_t_1(X) ;
Z = Y.get();

# Modifying any of X,Y or Z should modify the others.



