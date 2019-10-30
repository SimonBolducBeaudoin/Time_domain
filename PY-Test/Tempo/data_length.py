#!/bin/python
# -*- coding: utf-8 -*-

execfile("headers.py")

# Goal
# Iterate on L_data to check for errors and calculation time

# Instantification de X
L_kernel = 1<<8;
L_FFT = 1<<10;

exponant_min = 12 ;
exponant_max = 28 ;
L_data= 2**arange(exponant_min,exponant_max+1,2); 

mu = 0 ; 
sigma = 2**12 ;
print 'Voltage sample' ;
V_i = normal(mu,sigma, 2**exponant_max); V_i = int16(V_i) 


for l_data in L_data:
    print 'debut loop. l_data = {} '.format( l_data ) ;
    ## Je tests pour le calcul en float et en double
    FLOAT = Time_Quad_float_int16_t( L_kernel, l_data, L_FFT );
    DOUBLE = Time_Quad_double_int16_t( L_kernel, l_data, L_FFT);
    
    data_float = array(FLOAT.data, copy = False);
    data_float[:] = V_i[:l_data] ;
    data_double = array(DOUBLE.data, copy = False);
    data_double[:] = V_i[:l_data] ;

    FLOAT.execute_time = 0 ;
    DOUBLE.execute_time = 0 ;

    FLOAT.execute();
    DOUBLE.execute();

    print 'float  L_data= 2**{} : {} GB \t execution_time= {} s '.format( log(l_data,2) , (3*l_data*2/(10.0**9)) , FLOAT.execute_time/10.0**6) ;
    print 'double L_data= 2**{} : {} GB \t execution_time= {} s '.format( log(l_data,2)    , (3*l_data*4/(10.0**9)) , (DOUBLE.execute_time/10.0**6) ) ;

del FLOAT
del DOUBLE