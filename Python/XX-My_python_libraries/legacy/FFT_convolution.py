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