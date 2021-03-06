#!/bin/env/python
#! -*- coding: utf-8 -*-

'''
# Moments_cumulants.py

Goal : Veryfies that moment and cumulant functions are working as intended.
For
- uint histograms
- float hitograms

It calculates moment, centered moments and cumulant up to 3dr order and prints the resuts.
The input is a normal funciton generated by numpy random package.
'''
import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");
from Histograms import *

## Script specific imports
import scipy

## Experiment parameters
l_data= 1<<20;

## Instanciate TimeQuad object
mean = 0 ;
std_dev = 1.0;
max = 6*std_dev;
data_float = array(normal(mean,std_dev,l_data), dtype = 'float64');
'''data_uint = randint(0, (2**16), l_data,dtype='uint16');'''

## Homemade histograms
nofbins = 2**16;
H_float = Histogram_uint64_t(nofbins , 2);
'''H_uint = Histogram_uint64_t(2**16 , 36);''' 

H_float.accumulate(data_float, max = max)
'''H_uint.accumulate(data_uint);'''

## DISPLAY RESULTS
'''height = 8;'''

## Moments , centered moments and cumulants up to order 3
### float histogram
m_1 = H_float.moment_k_float( k = 1, max = max);
m_2 = H_float.moment_k_float( k = 2, max = max);
m_3 = H_float.moment_k_float( k = 3, max = max);

mu_2 = H_float.centered_moment_k_float( k = 2, max = max ); 
mu_3 = H_float.centered_moment_k_float( k = 3, max = max ); 

c_1 = H_float.cumulant_k_float( k = 1, max = max );
c_2 = H_float.cumulant_k_float( k = 2, max = max );
c_3 = H_float.cumulant_k_float( k = 3, max = max );

# bin_width = 2*max/nofbins;
# data_float = bin_width*floor(data_float/bin_width)

c_1_py = scipy.stats.moment(data_float,1)
c_2_py = scipy.stats.moment(data_float,2)
c_3_py = scipy.stats.moment(data_float,3)

print "float histogram"
print " m_1\t=\t{} \n m_2\t=\t{} \n m_3\t=\t{} \n mu_2\t=\t{} \n mu_3\t=\t{} \n c_1\t=\t{} \n c_2\t=\t{} \n c_3\t=\t{} \n c_1_py\t=\t{} \n c_2_py\t=\t{} \n c_3_py\t=\t{} \n".format(m_1,m_2,m_3,mu_2,mu_3,c_1,c_2,c_3, c_1_py,c_2_py,c_3_py)

print "Check that \n c_1 = m_1 = c_1_py \n c_2 = mu_2 = c_2_py \n c_3 = mu_3 = c_3_py"

### uint histogram
'''
m_1 = H_uint.moment_k( k = 1);
m_2 = H_uint.moment_k( k = 2);
m_3 = H_uint.moment_k( k = 3);

mu_2 = H_uint.centered_moment_k( k = 2); 
mu_3 = H_uint.centered_moment_k( k = 3); 

c_1 = H_uint.cumulant_k( k = 1);
c_2 = H_uint.cumulant_k( k = 2);
c_3 = H_uint.cumulant_k( k = 3);

c_1_py = scipy.stats.moment(data_uint,1)
c_2_py = scipy.stats.moment(data_uint,2)
c_3_py = scipy.stats.moment(data_uint,3)

print "uint histogram"
print " m_1\t=\t{} \n m_2\t=\t{} \n m_3\t=\t{} \n mu_2\t=\t{} \n mu_3\t=\t{} \n c_1\t=\t{} \n c_2\t=\t{} \n c_3\t=\t{} \n c_1_py\t=\t{} \n c_2_py\t=\t{} \n c_3_py\t=\t{} \n".format(m_1,m_2,m_3,mu_2,mu_3,c_1,c_2,c_3, c_1_py,c_2_py,c_3_py)
'''

