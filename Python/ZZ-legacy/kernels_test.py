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