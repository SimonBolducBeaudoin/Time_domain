#!/bin/python
# -*- coding: utf-8 -*-
execfile("../common_header.py")
from numpy import *

def DFT_freq ( index , l_dft = 257 , dt = 0.03125  ) :
    """
    Returns the ith frequency represented by a DFT with parameter l_dft and dt.
    This function doesn't check the parity of l_dft
    It also doesn't return negative value
    It also doesn't check if you're over Nyquist's frequency
    Those have to be checked by the user.
    See numpy.fft.fftfreq for an alternative
    """
    return index/(l_dft*dt) ;
    
def abscisse_half_complex( l_dft = 257 , dt = 0.03125 ):
    """
    Return the abscisse of a r2c fft of length l_dft and sampling rate dt 
    """
    l_hc = l_dft//2+1 ;
    Y = empty( l_hc , dtype = complex , order='C') ;
    for i in range(l_hc) :
        Y[i] = DFT_freq(i , l_dft , dt)
    
    return Y ;

def Gaussian ( x , mu = 0.0 , sigma = 1.0 ) :
    """
    Normalized gaussian function
    """
    return (1.0/(sigma*sqrt(2.0*pi))) * exp( (-(x-mu)**2)/(2.0*sigma**2) ) ;

def Gaussian_filter( f , df = 0.1 , l_kernel = 257 , dt = 0.03125 ) :
    """ 
    Returns a numpy array of complex number corresponding to a gaussian filter
    of avg f and std dev df on positive frequencies 
    and with vector length equal to  l_kernel//2 + 1. 
    This corresponds to a half complex vector.
    """
    l_hc = l_kernel//2+1 ;
    
    Y = empty( l_hc , dtype = complex , order='C') ;
    for i in range( l_hc ) :
        x_f = DFT_freq ( i , l_kernel , dt ) ;
        Y[i] =  Gaussian ( x_f , f , df ) ;
    
    return Y ;
    
def Bi_Gaussian_filter( f1 , f2 , df1=0.1 , df2 = 0.1 , l_kernel = 257 , dt = 0.03125 ) :
    """ 
    Returns a numpy array of complex number corresponding to a gaussian filter
    of avg f and std dev df on positive frequencies 
    and with vector length equal to  l_kernel//2 + 1. 
    This corresponds to a half complex vector.
    """
    l_hc = l_kernel//2+1 ;
    
    Y = empty( l_hc , dtype = complex , order='C') ;
    for i in range( l_hc ) :
        x_f = DFT_freq ( i , l_kernel , dt ) ;
        Y[i] =  Gaussian ( x_f , f1 , df1 ) + Gaussian(x_f , f2 , df2) ;
    
    return Y ;
    
def NoisySinGauss ( A = 1<<14 , f = 0.798132459087 , dt = 0.03125 , number_of_cycle_width =  10  , SNR = 10 ):
    # n : number of points of the output vector
    # A : Amplitude
    # f : [GHz]
    # dt : time step [ns] 
    # sigma_t : enveloppe width (std dev) on time axis
    # sigma_y : gaussian noise width (std dev)
    
    # check that n is odd
    sigma_t = number_of_cycle_width/f ; 
    n = int ( 10*sigma_t/dt ) ;
    if n//2 != 1 :
        n += 1 ;
        
    t = float64 ( (arange(n)-n//2)*dt ) ;
    
    Prefactor = 1 / (sigma_t * sqrt(2*pi) ) ;
     
    Enveloppe = Prefactor*exp( (-t**2)/(2*sigma_t**2) ) ;
    
    omega = 2*pi*f;
    
    return  A*Enveloppe*( sin(omega*t) + normal(0, 1/float(SNR),n) ) , t ; 

def WhiteNoise(A = 1<<14 , duree = 1000 , dt = 0.03125 ):
    # n : number of points of  the output vector
    # A : Amplitude
    
    n = int ( duree//dt ) ;
    t = float64 ( (arange(n)-n//2)*dt ) ;
    
    return  normal(0, A , n)  , t ; 

def SquareFunction(x,delta_x):
    answer = zeros(x.size);
    for  i , point in enumerate(x):
        if absolute(point) <= delta_x : answer[i] = 1/delta_x
    return answer  
    
def SinCycle ( f = 1 , dt = 0.03125 , n = 100 ):        
    t = float64 ( (arange(n)*dt ) ) ;
    
    omega = 2*pi*f;
    
    return  sin(omega*t) , t ; 
    
# Finds the indexes of the K largest element of X 
def KBestOfX3D(X,K):
    import heapq
    Shape = X.shape;
    Deroule = X.ravel() ;
    Taille = X.size ; 
    Index =  array( heapq.nlargest( K , range(Taille) , Deroule.__getitem__ ) ) ;
    index_i = Index//(Shape[1]*Shape[2]);
    index_j = (Index - (Shape[1]*Shape[2])*index_i)//(Shape[2]);
    index_k = Index - (Shape[1]*Shape[2])*index_i - (Shape[2])*index_j ;
    
    return (index_i , index_j , index_k)