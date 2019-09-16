#!/bin/python
# -*- coding: utf-8 -*-

import sys, os, platform, time

# Setting up the proper libraries and paths, mainly for Windows support
libpath = os.path.abspath(os.path.dirname(__file__))
plat_info = dict(plat=platform.system())
if plat_info['plat'] == 'Windows':
    plat_info['lib'] = os.path.join(libpath, 'time_domain.pyd')
    plat_info['com'] = 'make time_domain.pyd'
    # Adding cygwin libs path for windows
    libspath = 'C:\\cygwin64\\usr\\x86_64-w64-mingw32\\sys-root\\mingw\\bin'
    if libspath not in os.environ['PATH']:
        os.environ['PATH'] = libspath+os.path.pathsep+os.environ['PATH']   
else:
    plat_info['lib'] = os.path.join(libpath, 'time_domain.so')
    plat_info['com'] = 'make time_domain.so'

if not os.path.isfile(plat_info['lib']):
    raise IOError("{lib} is missing. To compile on {plat}:\n{com}\n".format(**plat_info))
    

