#!/bin/python
# -*- coding: utf-8 -*-

execfile("../common_header.py")

####################################
def Window_Tukey(x,alpha,x_max) :
    # https://en.wikipedia.org/wiki/Window_function
    L = x_max + 1 ;
    X_alpha = alpha*L/2.0;
    X = (x%x_max) ;
    if 0<=X and X < X_alpha:
        return 0.5 - 0.5*cos( pi*X/X_alpha );
    elif x_max-X_alpha<=X and X < x_max :
        return 0.5 - 0.5*cos( pi*(x_max-X)/X_alpha );
    else :
        return 1;
    
def WT (x,alpha,x_max) :
    result = zeros(len(x));
    for i,X in enumerate(x) :
        result[i] = Window_Tukey(X,alpha,x_max)
    return result

def Window_Tukey_modifed(x,x_1,x_2,x_max) :
    """
    1st transition stops at x_1
    2nd transition starts at x_2
    The function's period is x_max
    """
    L = x_max + 1 ;
    X = x%x_max if sign(x)== 1 else (-x)%x_max;
    
    if sign(X)==-1:
        return Window_Tukey_modifed(-X,x_1,x_2,x_max)
    elif 0<=X and X < x_1:
        return 0.5 - 0.5*cos( pi*X/x_1 );
    elif x_2<=X and X < x_max :
        return 0.5 - 0.5*cos( pi*(x_max-X)/(x_max-x_2) );
    else :
        return 1;

def WT_mod (x,x_1,x_2,x_max) :
    result = zeros(len(x));
    for i,X in enumerate(x) :
        result[i] = Window_Tukey_modifed(X,x_1,x_2,x_max)
    return result

####################################
# Measurement
dt = 0.03125; # [ns]; # Default value
l_kernel = (1<<8) + 1 ;
alpha = 0.75
####################################

# kernel_p = array(X.kernel_p, copy = False);
# kernel_q = array(X.kernel_q, copy = False);

l_kernel = 257 ; # Impaire
dt = 0.03125 ; # ns
f_Nyquist = 1/(2*dt);# GHz : 16 GHz
f_min = 1/(l_kernel*dt); # GHz : ~0.125 GHz
f_max_analogue = 10 ;# GHz
t_min_analogue = 1/f_max_analogue; # ns : 0.1 ns
f_min_analogue = 0.5;# GHz
t_max_analogue = 1/f_min_analogue; # ns : 2 ns


t = r_[-(l_kernel//2):l_kernel//2+1:]*dt;
t_pos = t + l_kernel//2*dt ;
t_half_pos = dt*( arange(l_kernel//2) + 1 ) ;
f = fftfreq(l_kernel,dt);

# Theoretical kernels
k_p_0 = 1/sqrt(absolute(t)); 
k_q_0 = sign(t)/sqrt(absolute(t));
where(k_p_0==inf,(2*sqrt(2))/sqrt(dt),k_p_0);
where(k_q_0==inf,0,k_q_0);

# Sampled kernels
k_q_1 , k_p_1 = ( 2.0/sqrt(t_half_pos) ) * (  fresnel( sqrt( (2.0/dt)*t_half_pos ) )  );
k_p_1 = concatenate( (k_p_1[::-1] , [2.0*sqrt(2.0)/sqrt(dt)] , k_p_1 ) ) ;
k_q_1 = concatenate( ( (-1)*k_q_1[::-1] , [0] , k_q_1 )) ;

# Time Windowed kernels
## Sampling equivalent filter and windowed with Tukey
alpha = 0.25 ;
Window = WT(t_pos,alpha,l_kernel*dt);
k_p_2 = Window*k_p_1 ;
k_q_2 = Window*k_q_1 ;

# Windowed 
Filter = WT_mod(f,0,f_max_analogue,f_Nyquist);
k_p_3 = Window*ifft ( Filter * fft(k_p_1) ) ;
k_q_3 = Window*ifft ( Filter * fft(k_q_1) );

# Windowed t&f DC removed
Filter = WT_mod(f,f_min_analogue,f_max_analogue,f_Nyquist);
k_p_4 = Window*ifft ( Filter * fft(k_p_1) ) ;
k_q_4 = Window*ifft ( Filter * fft(k_q_1) );

## plots
height = 8

fig , axs = subplots( 1,1, sharex = False)
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
axs.plot( t, k_p_0 , t , k_q_0)
axs.set(title = "Theoretical")

fig , axs = subplots( 1,1, sharex = False)
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
axs.plot( t, k_p_1 , t , k_q_1)
axs.set(title = "Sampled")

fig , axs = subplots( 1,1, sharex = False)
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
axs.plot( t, k_p_2 , t , k_q_2)
axs.set(title = "Windowed time")

fig , axs = subplots( 1,1, sharex = False)
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
axs.plot( t, k_p_3 , t , k_q_3)
axs.set(title = "Windowed t&f")

fig , axs = subplots( 1,1, sharex = False)
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
axs.plot( t, k_p_4, t , k_q_4)
axs.set(title = "Windowed t&f DC removed")

fig , axs = subplots( 1,1, sharex = False)
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
axs.plot( fftfreq(len(t),dt), fft(k_p_4), fftfreq(len(t),dt) , fft(k_q_4))
axs.set(title = "TF Windowed t&f DC removed")



# axs[0].plot( t, k_p_0 , t , k_q_0)
# axs[0].set(title = "t")
# axs[1].plot( f, fft(k_p_0) , t , fft(k_q_0))
# axs[1].set(title = "f")
# axs[0].plot( t, k_p_1 , t , k_q_1)
# axs[1].plot( t, k_p_2 , t , k_q_2)


