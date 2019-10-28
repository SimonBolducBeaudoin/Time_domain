#!/bin/python
# -*- coding: utf-8 -*-

execfile("headers.py")

# Instantification de X
l_data= 1<<16;
l_fft = 1<<8;
 

mu = 0 ; 
sigma = 2**12 ;
n_threads = 1 ; 

X = FFT_double_int16_t(l_data , l_fft, n_threads );
data = array(X.data, copy = False);
inplace_halfC = array(X.inplace_halfC, copy = False);
data[:] = normal(mu,sigma, l_data);

exponants =arange(7,10+1,1)
l_ffts= 2**exponants;

Executions = zeros( size(l_ffts) );

# Set timers to 0
for i , l_fft in enumerate(l_ffts):
    print "\n i = {}, l_fft = {}".format( i , l_fft);
    X.l_fft = l_fft ;
    ipython.magic ("timeit X.execute();");     
del X , data, inplace_halfC

# Points per time
# figure()
# plot(l_ffts , )
# title('Single core FFT')
# xlabel('log_2(FFT size)')
# ylabel(' execution time[$\mu$s]')


# Saving the experiment
# filename ="length_fft.dat";
# Experiment = "Multi_threads_fft"
# PC = "X1 Carbon i5-5200U"

# header = "Experiment, PC, branch_threads ,fftw_threads, NumberOfRep, avg_execution_time, OneBilion";
# 2dn line Extra comment
# header += "\nMulti threaded inplace fft on doubles ";

# f = open(filename , 'ab')
# f.close

