##############
# histogram 1D

fig , axs = subplots( 2,1, sharex = True)

fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)

axs[0].plot( H_abscisse, H_p.get() )
axs[0].set(title = "p")
axs[1].plot( H_abscisse , H_q.get() )
axs[1].set(title = "q")
################
# histogram 2D

# figure();
# imshow(H2D.get(),cmap='Reds', interpolation='none' , extent=[H2D_abscisse[0],H2D_abscisse[-1],H2D_abscisse[0],H2D_abscisse[-1]] );
# colorbar();

############
# fig , axs = subplots( 4,1, sharex = True)
# fig.subplots_adjust(hspace = 0.5 , wspace = 0)

# f_string  = "$f$ = {} [GHz] :  $l_k$ = {} \n $alpha$ = {} : SNR = {} ".format(f, l_kernel, alpha, SNR)

# fig.text(0.65, 0.89, f_string , fontsize=9)

# t_kernel    = (arange(l_kernel) - l_kernel//2)*dt ;
# t_valid     = (arange(p.size)   - p.size//2)  *dt ;

# axs[0].plot(t_kernel, NoyauP , t_kernel , NoyauQ)
# axs[0].set(title = "Kernels")
# axs[1].plot(t , data )
# axs[1].set(title = "Data")
# axs[2].plot(t_valid , p , t_valid , q)
# axs[2].set(title = "Quadratures")
# axs[3].plot(t_valid , sqrt( p**2 + q**2 ) )
# axs[3].plot( sqrt( p_full**2 + q_full**2 ) )
# axs[3].set(title = "Photon flux ")
# axs[3].set_xlabel(" t [ns]")