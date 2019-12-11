#!/bin/python
# -*- coding: utf-8 -*-
execfile("../common_header.py")

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

def Window_Tukey(alpha,N) :
    Y = ones(N);
    for i in arange( int(alpha*N/2) ):
       Y[i] = (1.0/2)*( 1+cos( 2*pi*i/(alpha*N) - pi )  )
    for i in arange( int(N*(1-alpha/2)) , int(N) ):
       Y[i] = (1.0/2)*( 1+cos( 2*pi*i/(alpha*N) - 2*pi/alpha + pi )  )
    return Y
    
def Kernels( l_kernel = 257 , dt = 0.3125 , alpha = 0.5) :
    # Returns the time kernel of p(t) and q(t)
    # Kernel de p(t) => 1/sqrt(|t|) 
    # Kernel de q(t) => sgn(t)/sqrt(|t|)
    
    # Units of time are [ns]
    # Units of frequencies are [GHz]
    
    # if l_kernel%2 != 1 : raise ValueError('l_kernel must be odd') ; 
    
    # # Paramètre du filtre temps/frequences
    # G = 1.0/(2.0*dt); # frequence de Nyquist / Borne supérieur de la bande passante numérique
    # F = (10.0/16.0) * G ; # Borne supérieur de la bande passante analogique
    
    # # Parameters of 2D Tukey filter (frequency part) 
    # a = 1/(G-F) ; # a > 0 
    # b = -1.0* F/(G-F); 
    
    # # devrait tjrs être un multiple de dt
    # I = dt*l_kernel/2.0 ; # Bornes supérieur de la bande passant numérique en temps
    # # H = 64.0*dt/2.0 ; # Bornes inférieur de la bande passant analogique en temps
    # H = 64.0*dt/1.0 ; # Bornes inférieur de la bande passant analogique en temps
    
    # # Parameters of 2D Tukey filter (time part) 
    # c = 1/(I-H)  ;
    # d = -1.0 * H/(I-H);
 
    # # vecteurs temps
    # t_H = arange( dt , H , dt ) ;
    # t_I = arange( t_H[-1]+dt , I , dt )  ;
 
    # # Kernels at t = 0
    # tmp = sqrt( 2.0*absolute(a) ) ;
    
    # ZERO_p = 2.0 *( sqrt(F) + sqrt(G) ) \
    # + 1/tmp \
    # * (\
    # cos(pi*b) * ( fresnel( tmp*sqrt(G))[1] - fresnel(tmp*sqrt(F))[1] ) \
    # - sin(pi*b) * sign(a) * ( fresnel( tmp*sqrt(G))[0] - fresnel(tmp*sqrt(F))[0] ) \
    # ) ;
    
    # ZERO_q = 0 ;
    
    
    # # Kernles at 0<t<H
        # # variables temporaire pour p
    # tmp = sqrt( absolute(t_H) )
    # p_tmp1 = ( 1.0/tmp ) * ( fresnel( tmp*sqrt(4.0*F) )[1]  + fresnel( tmp*sqrt(4.0*G) )[1] )
    
    # tmp = sqrt( 2.0 * absolute( a - 2.0*t_H) )
    # p_tmp2 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b)   * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # - sin(pi*b) * sign(a - 2.0*t_H) *( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # )
    
    # tmp = sqrt( 2.0 * absolute( a + 2.0*t_H) )
    # p_tmp3 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b) * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # - sin(pi*b) * sign(a + 2.0*t_H) *( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # )
    
        # # variables temporaire pour q
    # q_tmp1 = ( 1.0/sqrt(t_H) ) * ( fresnel( sqrt(4*F*t_H) )[0]  + fresnel( sqrt(4*G*t_H) )[0] )
    
    # tmp = sqrt( 2.0* absolute(a + 2.0 * t_H ) )
    # q_tmp2 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b)   * ( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # + sin(pi*b) * sign( a + 2.0 * t_H ) * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # )
    
    # tmp = sqrt(2.0* absolute(2.0* t_H - a) )
    # q_tmp3 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b) * ( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # - sin(pi*b) * sign(2.0* t_H - a) * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # )
    
        # # Combinaison des variables temporaires
    # p_H = p_tmp1 + p_tmp2 + p_tmp3 ;
    # q_H = q_tmp1 + q_tmp2 + q_tmp3 ;

    # # Kernles at H<t<I
        # # variables temporaire pour p
    # tmp = sqrt( absolute(t_I) )
    # p_tmp1 = ( 1.0/tmp ) * ( fresnel( tmp*sqrt(4.0*F) )[1]  + fresnel( tmp*sqrt(4.0*G) )[1] )
    
    # tmp = sqrt( 2.0 * absolute( a - 2.0*t_I) )
    # p_tmp2 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b)   * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # - sin(pi*b) * sign(a - 2.0*t_I) *( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # )
    
    # tmp = sqrt( 2.0 * absolute( a + 2.0*t_I) )
    # p_tmp3 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b) * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # - sin(pi*b) * sign(a + 2.0*t_I) *( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # )
    
        # # variables temporaire pour q
    # q_tmp1 = ( 1.0/sqrt(t_I) ) * ( fresnel( sqrt(4*F*t_I) )[0]  + fresnel( sqrt(4*G*t_I) )[0] )
    
    # tmp = sqrt( 2.0* absolute(a + 2.0 * t_I ) )
    # q_tmp2 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b)   * ( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # + sin(pi*b) * sign( a + 2.0 * t_I ) * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # )
    
    # tmp = sqrt(2.0* absolute(2.0* t_I - a) )
    # q_tmp3 = ( 1.0/tmp ) * \
    # ( \
    # cos(pi*b) * ( fresnel( tmp*sqrt(G) )[0] - fresnel( tmp*sqrt(F) )[0] ) \
    # - sin(pi*b) * sign(2.0* t_I - a) * ( fresnel( tmp*sqrt(G) )[1] - fresnel( tmp*sqrt(F) )[1] ) \
    # )
    
    # tmp4 = (1.0/2.0)*(1.0+cos( pi*c*t_I + pi*d ))
    # # tmp4 = (1.0)
        # # Combinaison des variables temporaires
    # p_I = tmp4 * ( p_tmp1 + p_tmp2 + p_tmp3 );
    # q_I = tmp4 * ( q_tmp1 + q_tmp2 + q_tmp3 );
    
    # # Partie positive des signaux
    # positive_p = concatenate( (p_H , p_I) ) ;
    # positive_q = concatenate( (q_H , q_I) ) ;
    
    # kernel_p = concatenate( (positive_p[::-1] , [ZERO_p] , positive_p ) ) ;
    # kernel_q = concatenate( ( (-1.0)*positive_q[::-1] , [ZERO_q] , positive_q )) ;
     
    # return kernel_p , kernel_q
    
    if l_kernel%2 != 1 : raise ValueError('l_kernel must be odd') ; 
    
    t_plus = dt*( arange(l_kernel//2) + 1 ) ;
    
    positive_q , positive_p = ( 2.0/sqrt(t_plus) ) * (  fresnel( sqrt( (2.0/dt)*t_plus ) )  );
    
    kernel_p = concatenate( (positive_p[::-1] , [2.0*sqrt(2.0)/sqrt(dt)] , positive_p ) ) ;
    kernel_q = concatenate( ( (-1)*positive_q[::-1] , [0] , positive_q )) ;
    
    Window = Window_Tukey( alpha , l_kernel ) ;
    kernel_p = Window*kernel_p ;
    kernel_q = Window*kernel_q ;
    
    kernel_p = Window * concatenate( (fft(kernel_p)[l_kernel//2+1:],fft(kernel_p)[:l_kernel//2+1]) )
    kernel_q = Window * concatenate( (fft(kernel_q)[l_kernel//2+1:],fft(kernel_q)[:l_kernel//2+1]) )
    
    kernel_p = ifft( concatenate( ( kernel_p[ l_kernel//2: ], kernel_p[:l_kernel//2] ) ) )
    kernel_q = ifft( concatenate( ( kernel_q[ l_kernel//2: ], kernel_q[:l_kernel//2] ) ) )
    
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