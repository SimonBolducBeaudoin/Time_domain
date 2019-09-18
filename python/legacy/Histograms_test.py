#!/bin/python
# -*- coding: utf-8 -*-

from  numpy import array, meshgrid, arange, zeros
from numpy.random import normal, randint
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d, Axes3D 
from PetQ_otf import *

from ctypes import c_uint64, c_uint32, c_uint16

from IPython import get_ipython
ipython = get_ipython()

# Params
L_data = (10**3) 
L_kernel = 10**2 # doit être paire
L_FFT = 1<<8    # idéalement une puissance de 2
Output_type = 1 # 0 : float ; 1: double(default)
Ouput_length = 0 # 0 : full ; 1: valid(default)
Convolution_type = 0 # 0 direct ; 1 : FFt (default)

#Initialize Conv_option structure
Conv_Opt = Options_init()
# Get current option dictionary 
Dict_Opt = Options_get_all(Conv_Opt)
# Update options 
setitems(Dict_Opt, ('L_kernel','L_data','L_FFT', 'Output_type', 'Output_length', 'Convolution_type'), (L_kernel, L_data, L_FFT, Output_type, Output_length, Convolution_type))
Options_set(Conv_Opt, **Dict_Opt) # This works if the dictornary contains one or more key and values.
Options_print(Conv_Opt) # Print the current state of the option structure
##

data = array(normal(0,1<<12,L), dtype = 'int16');

# Kernel ??





HH = zeros((k,k), dtype=c_uint64)
ipython.magic ("timeit hist.hist2dNbits(x, y, n, force_n=False, option='serial_uint8', ihist=HH, max=6)");


#H_x = hist.hist1dNbits(x, n,ihist=H, max=6);
#plt.plot(H_x);

#X, Y = meshgrid(arange(1<<n),arange(1<<n));



# fig = plt.figure();
# ax = Axes3D(fig);
# ax.plot_surface(X,Y,A,cmap='viridis');

# fig = plt.figure();
# ax = Axes3D(fig);
# ax.plot_surface(X,Y,B,cmap='viridis');


del x , H # 1d
#del H_x #1d graph 
del y, HH  #2d
#del X,Y, H_xy #2d graph