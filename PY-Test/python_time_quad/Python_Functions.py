#!/bin/python
# -*- coding: utf-8 -*-

# Fichier temporaire contenant les fonction pour QetPdeT/Accumul_Histo
from matplotlib.pyplot import plot, xlabel, ylabel, axis, hist, figure, axis, subplots, xlim, ylim

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