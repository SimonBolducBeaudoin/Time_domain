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