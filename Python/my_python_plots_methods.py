#!/bin/env/python
#! -*- coding: utf-8 -*-

def quick_plot_gz( Ys=[], Xs=[] , labels=[], x_label="" , y_label="", TR_comment="" ,height = 7 , Ratio_hw = 1.62):
    fig , axs = subplots(1,1)
    fig.set_size_inches( Ratio_hw*height , height )
    fig.text(0.74, 0.915, TR_comment , fontsize=9)
    if (Ys and Xs and labels) :
        for Y,X,label in zip(Ys,Xs,labels) :
            axs.plot( X , Y ,  label = label ) ;
        axs.set_xlabel(" Time [ns]")
    elif (Ys and labels) :
        for Y,label in zip(Ys,labels) :
            axs.plot( Y , label = label ) ;
        axs.set_xlabel(" Time [Samples]")
    elif Ys :
        for i,Y in enumerate(Ys) :
            axs.plot( Y , label = '{}'.format(i) ) ;
        axs.set_xlabel(" Time [Samples]")
    
    axs.set_ylabel(" V ")
    if x_label :
        axs.set_xlabel(x_label)
    if y_label :
        axs.set_xlabel(y_label)
    
    return fig , axs
    
def quick_plot_gz_hist1D( Ys=[] , labels=[], x_label="" , y_label="", TR_comment="" ,height = 7 , Ratio_hw = 1.62):
    fig , axs = subplots(1,1)
    fig.set_size_inches( Ratio_hw*height , height )
    fig.text(0.74, 0.915, TR_comment , fontsize=9)
        
    if (Ys and labels) :
        for Y,label in zip(Ys,labels) :
            axs.plot( Y , label = label ) ;
    elif Ys :
        for i,Y in enumerate(Ys) :
            axs.plot( Y , label = '{}'.format(i) ) ;
          
    axs.set_ylabel(" Counts ")
    axs.set_xlabel(" bin #")
    
    return fig , axs
    
def plot_PQvsVDC_Histo_1D_from_2D( Hs , Vdc , axis = 0 , x_label="" , y_label="", TR_comment="" ,height = 7 , Ratio_hw = 1.62 ):
    # One graph per filter
    for i in range(Hs.shape[1]):
        fig , axs = subplots(1,1)
        fig.set_size_inches( Ratio_hw*height , height )
        fig.text(0.74, 0.915, TR_comment , fontsize=9)
        for H,vdc in zip(Hs[:,i],Vdc) :
            label = "{:2.3F} [V]".format(vdc)
            axs.plot( H.get().sum(axis=axis) , label = label ) ;
        axs.set_ylabel(" Counts ")
        axs.set_xlabel(" bin #")
        axs.legend();
    
    
    
 ######
# 3 by 1 with top right comment and latex text

# s_hist_comment = "Hist : 2**16 bins  "
# s_gz_comment = "Gz : equalizer_en=False "
# s_date  = "Date (Begin) : 29-02-20 " 
# s_n_Sa  = "Sa/Histogram = {} [GSa] ".format( round(l_data*n_aquisitions/(4.0*10**9)) )

# s_tr = s_hist_comment + "\n" + s_gz_comment + "\n" + s_date + "\n" + s_n_Sa

# fig , axs = subplots( 3,1, sharey = True ,sharex = True)
# height = 7 ;
# Ratio_hw = 1.62 ;
# fig.set_size_inches( Ratio_hw2*height , height )
# fig.subplots_adjust(hspace = 0.5 , wspace = 0)
# fig.text(0.74, 0.915, s_tr , fontsize=9)

# axs[0].plot( diff_01 ) ;
# axs[0].set(title = "$\Delta_{01}$")
# axs[0].set_ylabel(" Counts ")
# axs[1].plot( diff_02 ) ;
# axs[1].set(title = "$\Delta_{02}$")
# axs[1].set_ylabel(" Counts ")
# axs[2].plot( diff_03 ) ;
# axs[2].set(title = "$\Delta_{03}$")
# axs[2].set_ylabel(" Counts ")
# axs[2].set_xlabel(" bin #")
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    