#!/bin/python
# -*- coding: utf-8 -*-

execfile("headers.py")

# Goal
# Iterate on L_data to check for errors and calculation time

# Instantification de X
L_data= 1<<16;
L_FFT = 1<<8;
 

mu = 0 ; 
sigma = 2**12 ;
N_threads = 6 ; 

X = FFT_Test_double_int16_t(L_data , L_FFT, N_threads );
data = array(X.data, copy = False);
inplace_halfC = array(X.inplace_halfC, copy = False);
data[:] = normal(mu,sigma, L_data);

exponants =arange(7,13+1,1)
L_FFTs= 2**exponants;

Executions = zeros( size(L_FFTs) );
OneBilion = zeros( size(L_FFTs) );

NumberOfRep = 10000; # in the c++ code

print "Loop starting"
# Set timers to 0
for i , l_FFT in enumerate(L_FFTs):
    print "i = {}, l_FFT = {}".format( i , l_FFT);
    X.L_FFT = l_FFT ;
    X.time_execution = 0 ;
    X.time_alloc = 0 ;
    X.time_preparation = 0 ;
    ipython.magic ("time X.prepare();");
    #X.prepare();
    ipython.magic ("time X.execute();");
    #X.execute();
    Executions[i] = X.time_execution;
    OneBilion[i] = (10**9)*float(X.time_execution)/(l_FFT*NumberOfRep) ;
    print 'single execution_time average (on {})= {} us'.format( NumberOfRep , X.time_execution/NumberOfRep) 
    print""
    
del X , data, inplace_halfC
# figure()
# plot(exponants,Executions/NumberOfRep)
# title('Single core FFT')
# xlabel('log_2(FFT size)')
# ylabel(' execution time[$\mu$s]')

# # Estimate time to process 10**9 points
# figure()
# plot(exponants,OneBilion)
# title('Estimate time  to process 10**9 points')
# xlabel('log_2(FFT size)')
# ylabel(' time[$\mu$s]')

# Saving the experiment
filename ="length_fft.dat";
Experiment = "Multi_threads_fft"
PC = "X1 Carbon i5-5200U"

header = "Experiment, PC, branch_threads ,fftw_threads, NumberOfRep, avg_execution_time, OneBilion";
# 2dn line Extra comment
header += "\nMulti threaded inplace fft on doubles ";

f = open(filename , 'ab')
f.close

