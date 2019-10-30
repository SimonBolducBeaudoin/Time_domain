# All the package stuff
    # Makes sur mingw libraries are in the PATH env. var.
import sys
sys.path.insert(0, 'C:/Users/simon/Desktop/CODES/Time_quadratures-OTF')
import add_mingw_to_PATH
    # load or curtom libraries
from time_quadratures import *; 
#import histograms_otf as hist
            
from  numpy import array, meshgrid, arange, zeros
from numpy.random import normal, randint
from matplotlib.pyplot import  plot, xlabel, ylabel, axis, hist, figure, axis, subplots, xlim, ylim, show
#from mpl_toolkits.mplot3d import axes3d, Axes3D 

# Functions
def MakeAPlotOf( *argv ): # Args are numpy arrays
    dt = 0.03125 #[ns]
    figure();
    for arg in argv:
        plot(arange(arg.size)*dt, arg, '.')
    show()

# Create Time_Quad object
L_kernel = 1<<8;
L_data= 1<<12;
L_FFT = 1<<10;

X = Time_Quad_double_int16_t( L_kernel, L_data, L_FFT, Convolution_type = 'fft');

# Numpy instance of arrays
data = array(X.data, copy = False);

kernel_p = array(X.kernel_p, copy = False);
kernel_q = array(X.kernel_q, copy = False);

p_full = array(X.p_full, copy = False);
#p_valid = array(X.p_valid, copy = False);

q_full = array(X.q_full, copy = False);
#q_valid = array(X.q_valid, copy = False);

#FFT_kernel_p = array(X.FFT_kernel_p, copy = False);
#FFT_kernel_q = array(X.FFT_kernel_q, copy = False);

# Check if FFT works on kernels
# FFT_kernel_p  = array(X.FFT_kernel_p(), copy = False);
# FFT_kernel_q  = array(X.FFT_kernel_q(), copy = False);

# Assign gaussian uint16 noise around 1<<16 / 2 
avg = 0;
sigma = 1<<12;

data[:] = normal(avg, sigma, L_data);
#data[:] = 1;

X.execute_kernel();

#X.Convolution_type ='direct';
#X.execute(); 

# Plots
PrintTheKernels = False;
if PrintTheKernels == True :
    MakeAPlotOf(kernel_p, kernel_q)

PrintTheData = False;
if PrintTheData == True :
    MakeAPlotOf(data)

PrintQP = False;
if PrintQP == True:
    MakeAPlotOf(p_full,q_full)
    
PrintFFT_Kernels = False;
if PrintFFT_Kernels == True:
    figure()
    plot(FFT_kernel_p.real)
    plot(FFT_kernel_p.imag)
    figure()
    plot(FFT_kernel_q.real)
    plot(FFT_kernel_q.imag)
    
    
# garbage
# del data, kernel_p, kernel_q, p_full, q_full, X