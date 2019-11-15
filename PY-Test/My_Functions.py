#!/bin/python
# -*- coding: utf-8 -*-
execfile("common_header.py")

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
    
def NoisySinSquare ( A = 1<<14 , f = sqrt(2) , dt = 0.03125 , number_of_cycle_width =  10  , SNR = 100 ):
    # n : number of points of the output vector
    # A : Amplitude
    # f : [GHz]
    # dt : time step [ns] 
    # sigma_t : enveloppe width (std dev) on time axis
    # sigma_y : gaussian noise width (std dev)
    
    # check that n is odd
    delta_t = number_of_cycle_width/f ; 
    n = int ( 4*delta_t/dt ) ;
    if n//2 != 1 :
        n += 1 ;
        
    t = float64 ( (arange(n)-n//2)*dt ) ;
     
    Enveloppe = SquareFunction(t, delta_t) ;
    
    omega = 2*pi*f;
    
    return  A*Enveloppe*( sin(omega*t) + normal(0, 1/float(SNR),n) ) , t ; 
    
def Kernels_impaires( l_kernel = 257 , dt = 0.3125 ) :
    # Returns the time kernel of p(t) and q(t)
    # Kernel de p(t) => 1/sqrt(|t|) 
    # Kernel de q(t) => sgn(t)/sqrt(|t|)
    
    f_min = 1/(l_kernel*dt);
    
    t_plus = dt*( arange(l_kernel//2) + 1 ) ;
    # positive_q = ( 2.0/sqrt(t_plus) ) * (  fresnel( sqrt( (2.0/dt)*t_plus ) )[0] - fresnel( sqrt( 4 * t_plus * f_min ) )[0] );
    # positive_p = ( 2.0/sqrt(t_plus) ) * (  fresnel( sqrt( (2.0/dt)*t_plus ) )[1] - fresnel( sqrt( 4 * t_plus * f_min ) )[1] );
    positive_q = ( 2.0/sqrt(t_plus) ) * (  fresnel( sqrt( (2.0/dt)*t_plus ) )[0]  );
    positive_p = ( 2.0/sqrt(t_plus) ) * (  fresnel( sqrt( (2.0/dt)*t_plus ) )[1]  );
    
    kernel_p = concatenate( (positive_p[::-1] , [2.0*sqrt(2.0)/sqrt(dt)] , positive_p ) ) ;
    kernel_q = concatenate( ( (-1)*positive_q[::-1] , [0] , positive_q )) ;

    # half_abscisse = dt*( arange(l_kernel//2) + 1 ) ;
    # half_kernel  = 1/sqrt( absolute( half_abscisse ) ) ;

    # kernel_p = concatenate( (half_kernel[::-1] , [2.0*sqrt(2.0)/sqrt(dt)]  , half_kernel ) ) ;
    # kernel_q = concatenate( ((-1)*half_kernel[::-1] , [0] , half_kernel ) ) ;

    return kernel_p, kernel_q

# Juste pour emballer les variables dans la fonction
def Beta_t(Df  = 0.005e9,N =  2**12,Fmin = 0.5e9,Fmax = 10e9) :
    from numpy import zeros, arange, concatenate
    from numpy import absolute as abs
    from numpy.fft import ifft       
    # Df : pas en fréquence [Hz]
    # N :  la FFT est maximalement efficace lorsque L=2^n
    # Fmin : [Hz] doit être un multiple de Df
    # Fmax : [Hz] doit être un multiple de Df
    # f = concatenate((0,arange(1,N+1),(arange(1,N+1))[::-1]*(-1)),axis=None)*Df
    t = concatenate((0,arange(1,N+1),(arange(1,N+1))[::-1]*(-1)),axis=None)*(1/(N*Df))
    
    # Construction de la représentation en fréquence
    FreqNulle = 0
    Index_ini = int(Fmin/Df) ; Index_fin = int(Fmax/Df); Largeur = int(Index_fin-Index_ini)
    FreqPos = zeros(N); FreqPos[Index_ini:Index_fin:1] = 1/(N*(Largeur))
    FreqNeg = FreqPos[::-1]
    Beta_f = concatenate((FreqNulle,FreqPos,FreqNeg),axis=None)
    
    # figure(figsize=(20,5))
    # plot(f*10**(-9),Beta_f,'*');xlabel('[GHz]');   
        
    InverseFFT = ifft(Beta_f)
    figure(figsize=(20,5))
    plot(t*10**9,abs(InverseFFT.real));xlabel('[ns]'); xlim(-1,1);
    
def Abscisse_Kernel_time(N,dt) :
    # Créer un vecteur t de t.size = 2(N//2) centrée en 0 et dont les éléments sont séparé par dt
    # S'assure de N est paire avec des opérations directes sur les bits
    import warnings
    if N & 1 :# est impaire
        N = N-1
        warnings.warn('Le nombre de points dans le kernel points doit être paire. N_k a été changé pour N_k-1.')
        
    from numpy import concatenate, arange
    return concatenate((-1*arange(1,N/2+1)[::-1],arange(1,N/2+1)),axis=None)*dt

def Abscisse_Kernel_freq(N,dt) :
    # Créer un vecteur f de f.size = 2*N dont le première élément est la fréquence nulle
    # les éléments f[1:N-1] sont les fréquence positives dont la valeur maximale est (1/dt)*N-1
    # les éléments f[N:] sont les fréquences négatives énumérées en sans croissant. La valeur la plus négatives étant (1/dt)*N
        
    from numpy.fft import fftfreq
    return fftfreq(N)*(1/dt) # Si nb de points paire max (freq_neq) \neq max(freq_pos)

def Kernels_time(N,dt) :
    # Returns the time kernel of p(t) and q(t)
    # Kernel de p(t) => 1/sqrt(|t|) 
    # Kernel de q(t) => sgn(t)/sqrt(|t|) 
    
    from numpy import sqrt, absolute, sign
    t = Abscisse_Kernel_time(N,dt)
    return 1/sqrt(absolute(t)),sign(t)/sqrt(absolute(t))

def Kernels_freq(N,dt) :
    # Returns the Kernel in the frequencie domaine for p(t) and q(t). The size of the kernel is 2N
    
    # S'assure de N est paire avec des opérations directes sur les bits
    if N & 1 :# est impaire
        N = N-1
    import warnings
    if N & 1 :# est impaire
        N = N-1
        warnings.warn('Le nombre de points dans le kernel points doit être paire. N_k a été changé pour N_k-1.')
    
    from numpy import sqrt, absolute, sign, sum, array, concatenate
    t = Abscisse_Kernel_time(N,dt)
    f = Abscisse_Kernel_freq(N,dt)
    
    Kernel_P = concatenate((sum(1/sqrt(absolute(t))),array(1/sqrt(absolute(f[1:])))),axis=None) 
    Kernel_Q =  (-1j)*concatenate((0,array(sign(f[1:])/sqrt(absolute(f[1:])))),axis=None) 
    return Kernel_P, Kernel_Q
    
def FFTConv(V_i, N, N_FFT, dt):
    # V_i : le np.array de mesure en tension
    # N : nombre de points dans les noyaux
    # dt : le pas en temps de la mesure
    
    import warnings
    if N & 1 :# est impaire
        N = N-1
        warnings.warn('Le nombre de points dans le kernel points être paire. N_k a été changé pour N_k-1.')

        
    from numpy import pad, zeros, array, delete
    from numpy.fft import rfft, irfft 
    
    # Attribues :
    # n = int(11); N_FFT = int(2**n); 
    N_vi = int(N_FFT - N + 1)
    N_Vi = int(V_i.size)
    
    # Méthodes :
    ## Calcul de la convolution
    ## Noyaux zero paddés
    (KernelP,KernelQ) = Kernels_time(N, dt)   # [ns^-1/2]
    KernelP = rfft(pad(KernelP,(0,N_FFT-N),'constant',constant_values = (0,0)))
    KernelQ = rfft(pad(KernelQ,(0,N_FFT-N),'constant',constant_values = (0,0)))
    ## Création de p_i et q_i de taille fixe
    N_chunk = int( N_Vi // N_vi + (1 if ( N_Vi % N_vi ) > 0  else 0))
    p_i = zeros([N_chunk,N_FFT])
    q_i = zeros([N_chunk,N_FFT])
    
    # Calculs des convolutions par morceaux
    for i in range( int(N_Vi//N_vi) ) : 
        v_i =    pad( V_i[ i*N_vi:(i+1)*N_vi ], (0,N_FFT-N_vi) , 'constant',constant_values = (0,0) ) 
        p_i[i,:] = irfft(KernelP*rfft(v_i)) 
        q_i[i,:] = irfft(KernelQ*rfft(v_i)) 
    ## Traitement du reste
    N_r = N_Vi%N_vi
    if N_r > 0 :        
        v_i = pad(V_i[-N_r:],(0,N_FFT-N_r),'constant',constant_values = (0,0)) 
        p_i[-1,:] = irfft( KernelP*rfft(v_i) ) 
        q_i[-1,:] = irfft( KernelQ*rfft(v_i) ) 
    
    # Reconstitution de P_i et Q_i à partie de p_i et q_i
    ## Declaration de P_i et Q_i de taille fixe
    P_i = zeros((N_chunk-1)*N_vi+N_FFT)
    Q_i = zeros((N_chunk-1)*N_vi+N_FFT)
    
    for i in range(N_chunk): 
        P_i[i*N_vi:(i*N_vi)+N_FFT] += p_i[i,:]
        Q_i[i*N_vi:(i*N_vi)+N_FFT] += q_i[i,:]

    # Enlver les zéros en trop à la fin de P_i et Q_i provenant du traitement du reste 
    if N_r != 0 : 
        Fin = slice(-(N_vi-N_r),None,1)
        P_i = delete(P_i,Fin)
        Q_i = delete(Q_i,Fin)
        
    # Sélectionner seulement la partie «valid» du produit de convolution
    #Debut = slice(None,N-1,1); 
    #Fin = slice(-(N-1),None,1)
    #P_i = delete(P_i,Debut)
    #P_i = delete(P_i,Fin)
    #Q_i = delete(Q_i,Debut)
    #Q_i = delete(Q_i,Fin)
    
    return P_i, Q_i

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