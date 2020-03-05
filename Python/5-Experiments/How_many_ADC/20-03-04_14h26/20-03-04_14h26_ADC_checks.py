#!/bin/env/python
#! -*- coding: utf-8 -*-

"""
    Get:
        - many long aquisition from Guzik
        - Hypothesis : I suppose that I always start on the same ACD
        - Plot n histograms 
            - 1st Hist of data[0:-1:n]
            - 2nd Hist of data[1:-1:n]
            - ect.
            - nth Hist of data[n-1:-1:n]
        - Equalizer is turned off
"""

################################
## TO FILL AT SCRIPT CREATION ##
################################
time_stamp_start = time.strftime('%y-%m-%d_%Hh%M')
path_test = "C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\TEST" # Common test folder for all experiments

test = False ; # By default test should be true
path_script = "C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\How_many_ADC" ;
path_current = path_script if not(test) else path_test
filename = "ADC_checks.py"

make_dir(path_current + "\\" + time_stamp_start ) # pyHegel's function to create dir and changes sweep.path to point there
path_save = get(sweep.path); # The script is going to be saved at the same location as wher pyHegel saves stuff.

########################
## APPEND PARRENT DIR ##
########################
import os, sys
parentdir = os.path.abspath(os.path.join(os.getcwd(), os.pardir)) ;
sys.path.append(parentdir)

#############
## IMPORTS ##
#############
execfile("../common_header.py");
from pyHegel import *
from my_python_library import *
from my_pyHegel_library import *
from my_python_plots_methods import*

from Histograms import *

##############################################
## SCRIPT COPY SHOULD BE ADDED TO A LIBRARY ##
##############################################
"""
FUTUR ? : Also copy script from my custom librarie to where the data are saved
"""

def Copy_script(path_original,path_copy):
    """
    example :
    path_original = C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\PQsVsVDC\\PQsvsVDC.py
    path_copy = C:\\Projets\\Time_Domain_Optic\\Time_domain\\Python\\5-Experiments\\TEST\\PQsvsVDC.py
    """
    with open(path_original, 'r') as f:
        with open(path_copy, 'w') as out:
            for line in (f.readlines()): #remove last 7 lines
                out.write(line)

Copy_script( path_script+"\\"+ filename , path_save+"\\"+ time_stamp_start + "_" + filename )

#################################################
## GUZIK SPECIFIC SHOULD BE ADDED TO A LIBRARY ##
#################################################

def load_guzik():
    try:
        if not isinstance(gz, instruments.guzik.guzik_adp7104):
            print "\nLoading guzik :"
            gz = instruments.guzik_adp7104()
    except:
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
        
    return gz
    
def gz_quick_hist(use_cache=False):
    H = Histogram_uint64_t( 2**16 , 36 );
    
    data = get(gz_snippet, snipsize=10**6).astype('float64'); # En attendant de correctement implémenter les histograms de int16
    H.accumulate(data, max = 2**15) ;
    
    quick_plot_gz_hist1D( [H.get()] )

#############################################################
##  WRAPPER FUNCTIONS SHOULD BE REPLACED BY WRAPPER CLASS ##
#############################################################
# GUZIK SPECIFIC 
def gz_snippet_wrapper( snipsize=1000):
    v = get(gz)
    snip = v[:snipsize].copy()
    return snip

#############################
## EXPERIMENTAL PARAMETERS ##
#############################
n_aquisitions = 2*10*16 ; # The number of aquisition made with Guzik
l_data= 2**26 ; # The length of one aquisition
gain_dB = 11.0 ; # Warning : choose this wisely before running

# Algorithm params
n_threads = 36 ;

n_Hs = 16 ; # data will be sliced in 16 hist

l_data= n_Hs *(l_data//n_Hs) # Makes sure l_data is divisable by n_Hs

########################
## INITIALIZING STUFF ##
########################
nb_of_bin = 2**16 ;
Hs = r_[[ Histogram_uint64_t( nb_of_bin , n_threads ) for i in range(n_Hs)]] ;

# allocate memory
data = empty (l_data , dtype = 'float64')
data_slice = empty((n_Hs,l_data//n_Hs), dtype = 'float64' );

gz = load_guzik() ;
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=False, ext_ref='int')

########################
## VIRTUAL INSTRUMENTS STUFF ##
########################
gz_snippet = instruments.FunctionWrap(getfunc=gz_snippet_wrapper, basedev=gz.fetch, multi=('GZ_Snippet',))

#####################
## MEASURING STUFF ##
#####################
t_begin = time.time(); 
for j in range(n_aquisitions):
    print "{}/{}".format(j,n_aquisitions)
    data[:] = get( gz ).astype('float64'); # En attendant de correctement implémenter les histograms de int16
     
    for i in range (n_Hs) : 
        data_slice[i,:] = numpy.copy( data[i::n_Hs] );
    
    for i in range (n_Hs) :
        Hs[i].accumulate(data_slice[i,:], max = 2**15) # because int16 accumulate is not working rigth now
t_end = time.time(); 
Estimated_time_per_loop = 30.0/16.0 ;
print("Experience over : \n \t Estimated time [s] {:04.2F} \n \t Real time {:04.2F} [s] \n ").format( n_measures*Estimated_time_per_loop , t_end-t_begin )
    
#######################################################
## CONVERT AND COPY ARRAY OF HISTOGRAMS TO BIG ARRAY ##
#######################################################
HS = r_[ [Hs[i].get() for i in range(n_Hs) ] ]
HS_shape = ( n_Hs , nb_of_bin ) ;
HS = HS.reshape( HS_shape ) ;

##################
## SAVING STUFF ##
##################
def save_data(path_save):    
    """
    Futur write a quick_save_data(path_save)
        that saves in .npy
    """
    time_stamp = time.strftime('%y-%m-%d_%Hh%M') # File name will correspond to when the experiment ended
    
    savez_compressed(os.path.join(path_save, 'HS{}_{}.npz'.format('' if not(test) else '_test', time_stamp)), HS) 
    
save_data(path_save);

def plot_Hist_1D_in_subfig( HS , x_label="bin#" , y_label="Counts", TR_comment="" ,height = 10 , Ratio_hw = 1.62 ):    
    fig , axs = subplots(HS.shape[0]/2,2 ,sharey = True ,sharex = True)
    fig.set_size_inches( Ratio_hw*height , height )
    fig.text(0.74, 0.915, TR_comment , fontsize=9)
    for i in range(HS.shape[0]/2):
        for j in range(2):
            label = "[{}] ".format(j+2*i)
            axs[i,j].plot( HS[j+2*i,:] , label = label ) ;
            axs[i,j].legend();
    # axs.set_ylabel(y_label)
    # axs.set_xlabel(x_label)
    return fig , axs

plot_Hist_1D_in_subfig(HS);

def plot_diff_Hist_1D_in_subfig( HS , x_label="bin#" , y_label="Counts", TR_comment="" ,height = 10 , Ratio_hw = 1.62 ):    
    fig , axs = subplots(HS.shape[0]/2,2 ,sharey = True ,sharex = True)
    fig.set_size_inches( Ratio_hw*height , height )
    fig.text(0.74, 0.915, TR_comment , fontsize=9)
    for i in range(HS.shape[0]/2):
        for j in range(2):
            label = "[{}] ".format(j+2*i)
            axs[i,j].plot( HS[0,:].astype('int64') - HS[j+2*i,:].astype('int64') , label = label ) ;
            axs[i,j].legend();
    # axs.set_ylabel(y_label)
    # axs.set_xlabel(x_label)
    return fig , axs

plot_diff_Hist_1D_in_subfig(HS)
