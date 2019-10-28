# PATH management stuff
import sys
sys.path.insert(0, 'C:/Users/simon/Desktop/CODES/Time_Domain/FFT_benchmarks')# pc simon-i5_5200U
# sys.path.insert(0, 'C:/Users/Sous-sol/Desktop/CODES/Time_Domain/Time_quadratures-OTF')# pc simon-Ryzen-1600X
# sys.path.insert(0, 'C:/Projets/Time_Domain_Optic/Time_Domain/Time_quadratures-OTF')
import add_mingw_to_PATH

# plot stuff
from matplotlib.pyplot import ( plot, xlabel, ylabel, axis, hist, figure, show,  axis, subplots, xlim, ylim, title, semilogx, legend, savefig )
from mpl_toolkits.mplot3d import axes3d, Axes3D 
# timeit funcitonnalities stuff
from IPython import get_ipython
ipython = get_ipython()
# NUMPY
from numpy import (array, meshgrid, arange, zeros, array_equal, around
                   , convolve , int16 , size , savez, log2 )
from numpy.fft import fft, fftfreq
from numpy.random import normal, randint 

# SCIPY 
from scipy.signal import fftconvolve


# Garbage collection
import gc

# CUSTOM PY MODULE stuff
from Python_Functions import*
# CPP Time_Quad stuff
from fft_benchmark import*