#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from Fast_convolution import *

####################################
## Setup variables
l_kernel = 1<<8;
l_data= 1<<28;
l_fft = 1<<9;
n_threads = 2;
convolution_type = 'fft';
####################################

L_data = 2**array([24,25,26,27,28,29,30]);
# N_threads = array([36,40,50,60,70,72]);
N_threads = linspace(24,72,24+1).astype(int);
# L_FFTs = 2**array([8,9,10,11,12,13,14,15,16,17,18]);
# L_data = 2**array([24,26]);
# N_threads = array([36,50]);
L_FFTs = 2**array([9,10,11,12,13]); 

# Matrice de taux de traitement
PointsPerSec_f = zeros( (size(L_data), size(N_threads) , size(L_FFTs) ) ) ;
PointsPerSec_d = zeros( (size(L_data), size(N_threads) , size(L_FFTs)) ) ;
ExecutionTime_f = zeros( (size(L_data), size(N_threads) , size(L_FFTs) ) ) ;
ExecutionTime_d = zeros( (size(L_data), size(N_threads) , size(L_FFTs) ) ) ;
ExecutionTime_d = zeros( (size(L_data), size(N_threads) , size(L_FFTs) ) ) ;


for i , l_data in enumerate(L_data):
    print "\t l_data = 2**{}".format(log2(l_data));
    # Initialization de l'objet de convolution rapide
    X_f  = FastConvolution_float_float_int16_t_complex_f( l_kernel, l_data, l_fft, convolution_type , n_threads );
    X_d  = FastConvolution_double_double_int16_t_complex_d( l_kernel, l_data, l_fft, convolution_type , n_threads );
    
    # Variable interne du timer
    X_f.time_execution = 0 ;
    X_d.time_execution = 0 ;
    for j , n_threads in enumerate(N_threads):
        print "\t\t n_threads = {}".format(n_threads);
        X_f.n_threads=n_threads;
        X_d.n_threads=n_threads;
        for k, l_fft in enumerate(L_FFTs):
            print "\t\t\tl_fft = {}".format(l_fft);
            X_f.l_fft = l_fft;
            X_d.l_fft = l_fft;
            print "execution "
            # ipython.magic ("time  X_f.execute();");
            # ipython.magic ("time  X_d.execute();");
            Repetitions = 10;
            for l in arange(Repetitions):
                X_f.execute();
                X_d.execute();
            
            t_f = X_f.time_execution/Repetitions ;
            t_d = X_d.time_execution/Repetitions ;
            ExecutionTime_f[i,j,k] = t_f; 
            ExecutionTime_d[i,j,k] = t_d ; 
            PointsPerSec_f[i,j,k] = 10**(-3)*l_data/t_f ; 
            PointsPerSec_d[i,j,k] = 10**(-3)*l_data/t_d;
            
            # Reset internal timer
            X_f.time_execution = 0 ;
            X_d.time_execution = 0 ;
    
    del X_f
    del X_d
    
# Print best conditions and outputs the tables into a .txt file
KK = 20 ;
benchname = 'FastConv_benchmark_fft.txt'
with open(benchname, 'w') as f:
    messages = ['float\n\t log2(l_data) \t Time [ms] \t n_threads  \t log2(l_fft)  \t G Points/sec \n',] ;
    (i,j,k) = KBestOfX3D(PointsPerSec_f, KK);
    for I,J,K in zip(i,j,k):
        messages += ["\t {0:.0f} \t\t {1:.1f} \t\t  {2:.0f} \t\t {3:.0f} \t\t {4:.2f}  \n".format( log2(L_data[I]) , ExecutionTime_f[I,J,K]/10**3 , N_threads[J] , log2(L_FFTs[K]) , PointsPerSec_f[I,J,K] ),] ;
    (i,j,k) = KBestOfX3D(PointsPerSec_d, KK);
    messages += ['double\n\t log2(l_data) \t Time [ms] \t n_threads  \t log2(l_fft)  \t [GSa/s] \n',] ;
    for I,J,K in zip(i,j,k):
        messages += ["\t {0:.0f} \t\t {1:.1f} \t\t  {2:.0f} \t\t {3:.0f} \t\t {4:.2f}  \n".format( log2(L_data[I]) , ExecutionTime_d[I,J,K]/10**3 , N_threads[J] , log2(L_FFTs[K]) , PointsPerSec_d[I,J,K] ),] ;
    f.writelines(messages)    
      
# filename = "FastConvolution_benchmark.npz" 
# savez(filename, L_data = L_data ,N_threads = N_threads , L_FFTs = L_FFTs , 
# ExecutionTime_f = ExecutionTime_f , ExecutionTime_d = ExecutionTime_d,
# PointsPerSec_f = PointsPerSec_f , PointsPerSec_d = PointsPerSec_d  ) ;


