#!/bin/env/python
#! -*- coding: utf-8 -*-

"""
    This code is used to check how many ADC the aquisition card has.
    
    RESULTS :
    Guzik as 2 ADC with slitly different response. 
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)

execfile("../common_header.py");
from Histograms import *

## Experiment parameters
    # TimeQuad params
l_data= 2**28;
    # Guzik params
gain_dB = 11.0 ; # Warning : choose this wisely before running

## Loading GUZIK
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()
    
gz.config([1], n_S_ch=l_data, gain_dB=gain_dB, bits_16=True, equalizer_en=False, ext_ref='int')

H_0 = Histogram_uint64_t( 2**16 , 36 );
H_1 = Histogram_uint64_t( 2**16 , 36 );
H_2 = Histogram_uint64_t( 2**16 , 36 );
H_3 = Histogram_uint64_t( 2**16 , 36 );

## The EXPERIMENT
n_aquisitions = 9836 ;
for j in range(n_aquisitions):
    print "{}/{}".format(j,n_aquisitions)
    data = get( gz ).astype('float64'); # En attendant de correctement implémenter les histograms de int16
     
    data_0 = numpy.copy( data[0::4] );
    data_1 = numpy.copy( data[1::4] );
    data_2 = numpy.copy( data[2::4] );
    data_3 = numpy.copy( data[3::4] );

    H_0.accumulate(data_0, max = 2**15)
    H_1.accumulate(data_1, max = 2**15) 
    H_2.accumulate(data_2, max = 2**15)
    H_3.accumulate(data_3, max = 2**15)

## PLOTS
R_0 = H_0.get().astype('float')
R_1 = H_1.get().astype('float')
R_2 = H_2.get().astype('float')
R_3 = H_3.get().astype('float')

diff_01 = R_0-R_1;
diff_02 = R_0-R_2;
diff_03 = R_0-R_3;

prob_diff_01 = diff_01/(R_0+R_1)
prob_diff_02 = diff_02/(R_0+R_2)
prob_diff_03 = diff_03/(R_0+R_3)

## Top right comment
s_hist_comment = "Hist : 2**16 bins  "
s_gz_comment = "Gz : equalizer_en=False "
s_date  = "Date (Begin) : 29-02-20 " 
s_n_Sa  = "Sa/Histogram = {} [GSa] ".format( round(l_data*n_aquisitions/(4.0*10**9)) )

s_tr = s_hist_comment + "\n" + s_gz_comment + "\n" + s_date + "\n" + s_n_Sa

### Differential count
fig , axs = subplots( 3,1, sharey = True ,sharex = True)
height = 7 ;
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
fig.text(0.74, 0.915, s_tr , fontsize=9)

axs[0].plot( diff_01 ) ;
axs[0].set(title = "$\Delta_{01}$")
axs[0].set_ylabel(" Counts ")
axs[1].plot( diff_02 ) ;
axs[1].set(title = "$\Delta_{02}$")
axs[1].set_ylabel(" Counts ")
axs[2].plot( diff_03 ) ;
axs[2].set(title = "$\Delta_{03}$")
axs[2].set_ylabel(" Counts ")
axs[2].set_xlabel(" bin #")

### Normalized differential count
s_formula = r"$\frac{C_0 - C_n}{0.5(C_0 + C_n)}$"

fig , axs = subplots( 3,1, sharey = True ,sharex = True)
height = 7 ;
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
fig.text(0.74, 0.915, s_tr , fontsize=9)

X = arange(nb_of_bin);
axs[0].plot( X[ ~numpy.isnan(prob_diff_01) ], prob_diff_01[~numpy.isnan(prob_diff_01)] , label = s_formula) ;
axs[0].legend()
axs[0].set(title = "$\Delta_{01}$")
axs[0].set_ylabel("Diff. Prob.")
axs[1].plot( X[ ~numpy.isnan(prob_diff_02) ], prob_diff_02[~numpy.isnan(prob_diff_02)] ) ;
axs[1].set(title = "$\Delta_{02}$")
axs[1].set_ylabel("Diff. Prob.")
axs[2].plot( X[ ~numpy.isnan(prob_diff_03) ], prob_diff_03[~numpy.isnan(prob_diff_03)] ) ;
axs[2].set(title = "$\Delta_{02}$")
axs[2].set_ylabel("Diff. Prob.")
axs[2].set_xlabel(" bin #")

# histograms
fig , axs = subplots( 1,1)
height = 7 ;
fig.set_size_inches( 1.62*height , height )
fig.subplots_adjust(hspace = 0.5 , wspace = 0)
fig.text(0.74, 0.915, s_tr , fontsize=9)

X = arange(nb_of_bin);
axs.plot( X[ numpy.not_equal(R_0,0.0) ], R_0[ numpy.not_equal(R_0,0.0) ] , label = "1st ADC") ;
axs.plot( X[ numpy.not_equal(R_1,0.0) ], R_1[ numpy.not_equal(R_0,0.0) ] , label = "2nd ADC") ;
axs.legend()
axs.set_ylabel(" Counts ")
axs.set_xlabel(" bin #")




