#!/bin/python
# -*- coding: utf-8 -*-
#!/bin/python
# -*- coding: utf-8 -*-

import sys, os, platform, time
# Setting up the proper libraries and paths, mainly for Windows support
libpath = os.getcwd();
# libpath = os.path.abspath(os.path.dirname(__file__))
plat_info = dict(plat=platform.system())
if plat_info['plat'] == 'Windows':
    # plat_info['lib'] = os.path.join(libpath, 'time_quadratures.pyd')
    # plat_info['com'] = 'make time_quadratures.pyd'
    # Adding cygwin libs path for windows
    libspath = 'C:\\cygwin64\\usr\\x86_64-w64-mingw32\\sys-root\\mingw\\bin'
    if libspath not in os.environ['PATH']:
        os.environ['PATH'] = libspath+os.path.pathsep+os.environ['PATH']   
# else:
    # plat_info['lib'] = os.path.join(libpath, 'time_quadratures.so')
    # plat_info['com'] = 'make time_quadratures.so'

# if not os.path.isfile(plat_info['lib']):
    # raise IOError("{lib} is missing. To compile on {plat}:\n{com}\n".format(**plat_info))
    
# plot stuff
from matplotlib.pyplot import ( plot, xlabel, ylabel, axis, hist, figure, show,  axis, subplots, xlim, ylim, title, semilogx, legend, savefig )
from mpl_toolkits.mplot3d import axes3d, Axes3D 
# timeit funcitonnalities stuff
from IPython import get_ipython
ipython = get_ipython()
# NUMPY
from numpy import (array, meshgrid, arange, zeros, ones , array_equal, around
                   , convolve , int16 , float64 , size , savez, log2
                   , array_equal , allclose , sin, cos , tan , exp
                   , sqrt, absolute, sign , pi, concatenate
                   , mean , around , vectorize)
from numpy.fft import fft, ifft, rfft, irfft, fftfreq
from numpy.random import normal, randint 

# SCIPY 
from scipy.signal import fftconvolve
from scipy.special import fresnel

#math
from math import pi

# Garbage collection
import gc

# CUSTOM PY MODULE stuff
from My_Functions import*