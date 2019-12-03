#!/bin/env/python
#! -*- coding: utf-8 -*-

execfile("../common_header.py");

## Loading devices
    # Only loads Guzik if it doesnt already exist.
try:
    if not isinstance(gz, instruments.guzik.guzik_adp7104):
        print "\nLoading guzik :"
        gz = instruments.guzik_adp7104()
except:
    print "\nLoading guzik :"
    gz = instruments.guzik_adp7104()

    # Configures (could be called many times in the same scrip)
    # WARNING : always start by configuring the Guzik with low gain (I think it can even be negative?) to avoid damaging the aquisition card.
gz.config(channels=[1], n_S_ch=1024, bits_16=True, gain_dB=0.0, equalizer_en=True, ext_ref='default')

## Example of a virtual instrument 

    # This is going to be our get function
def gz_snippet_wrapper(snipsize=1000):
    v = get(gz)
    snip = v[:snipsize].copy()
    return snip
    
    # Then we declare the actual virtual instrument
gz_snippet = instruments.FunctionWrap(getfunc=gz_snippet_wrapper, basedev=gz.fetch, multi=('GZ_Snippet',))
    # Try : instruments.FunctionWrap? for documentation

# The EXPERIMENT
data = get( gz_snippet );

# Plot or Save the Data
figure()
plot(data)

