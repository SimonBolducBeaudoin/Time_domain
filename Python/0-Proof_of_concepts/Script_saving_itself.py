#!/bin/env/python
#! -*- coding: utf-8 -*-

"""
Simplpe script that 
    - aquires data from guzik
    - calculates p and q for many input filters
    - Accumulates histograms of ps and qs
    - Calculates culants
    - Displays expval of n and n^^2
"""

import os, sys
currentdir = os.getcwd();
parentdir = os.path.abspath(os.path.join(currentdir, os.pardir)) ;
sys.path.append(parentdir)


execfile("../common_header.py");


from time_quadratures import *
from Histograms import *

test = False


def Copy_This_script():
    # not included in output file
    out_filename = ('{}_{}'.format("filename","current_time"))

    with open(__file__, 'r') as f:
        with open(out_filename, 'w') as out:
            for line in (f.readlines()): #remove last 7 lines
                out.write(line)
                
Copy_This_script()