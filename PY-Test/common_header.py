#!/bin/python
# -*- coding: utf-8 -*-

import sys, os, platform, time
# Setting up the proper libraries and paths, mainly for Windows support
libpath = os.getcwd();
# libpath = os.path.abspath(os.path.dirname(__file__))
plat_info = dict(plat=platform.system())
if plat_info['plat'] == 'Windows':
    plat_info['lib'] = os.path.join(libpath, 'fft_benchmark.pyd')
    plat_info['com'] = 'make fft_benchmark.pyd'
    # Adding cygwin libs path for windows
    libspath = 'C:\\cygwin64\\usr\\x86_64-w64-mingw32\\sys-root\\mingw\\bin'
    if libspath not in os.environ['PATH']:
        os.environ['PATH'] = libspath+os.path.pathsep+os.environ['PATH']   
else:
    plat_info['lib'] = os.path.join(libpath, 'fft_benchmark.so')
    plat_info['com'] = 'make fft_benchmark.so'

if not os.path.isfile(plat_info['lib']):
    raise IOError("{lib} is missing. To compile on {plat}:\n{com}\n".format(**plat_info))
    
# plot stuff
from matplotlib.pyplot import ( plot, xlabel, ylabel, axis, hist, figure, show,  axis, subplots, xlim, ylim, title, semilogx, legend, savefig )
from mpl_toolkits.mplot3d import axes3d, Axes3D 
# timeit funcitonnalities stuff
from IPython import get_ipython
ipython = get_ipython()
# NUMPY
from numpy import (array, meshgrid, arange, zeros, array_equal, around
                   , convolve , int16 , size , savez, log2
                   , array_equal , allclose )
from numpy.fft import fft, rfft, fftfreq
from numpy.random import normal, randint 

# SCIPY 
from scipy.signal import fftconvolve

# Garbage collection
import gc

# CUSTOM PY MODULE stuff
from Python_Functions import*