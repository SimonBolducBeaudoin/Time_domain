# PATH management stuff
import sys
sys.path.insert(0, 'C:/Users/simon/Desktop/CODES/Time_domain')# pc simon-i5_5200U
#sys.path.insert(0, 'C:/Users/Sous-sol/Desktop/CODES/Time_quadratures-OTF')# pc simon-Ryzen-1600X
sys.path.insert(0, 'C:/Projets/Time_Domain_Optic/Time_Domain')
import add_mingw_to_PATH

# plot stuff
from matplotlib.pyplot import  plot, xlabel, ylabel, axis, hist, figure, axis, subplots, xlim, ylim, title, semilogx
from mpl_toolkits.mplot3d import axes3d, Axes3D 
# timeit funcitonnalities stuff
from IPython import get_ipython
ipython = get_ipython()
# NUMPY
    # np utilities stuff
from numpy import array, meshgrid, arange, zeros, array_equal, around
    # np convolution stuff
from numpy import convolve
    # np FFT stuff
from numpy.fft import fft, fftfreq
    # np random stuff
from numpy.random import normal, randint
    # numpy type stuff
from numpy import  int16
# SCIPY 
from scipy.signal import fftconvolve

#MATH
from math import log

# CUSTOM PY MODULE stuff
from Python_Functions import*
# CPP Time_Quad stuff
from time_domain import*