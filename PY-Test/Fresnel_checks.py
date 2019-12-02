#!/bin/python
# -*- coding: utf-8 -*-

execfile("common_header.py")

# Dont forget to drop your library.pyd in the current folder
from time_quadratures import *

X = linspace(0.1,10,100)

fresnel_cos_cpp = zeros(X.size);
fresnel_cos_py = zeros(X.size);
fresnel_sin_cpp = zeros(X.size);
fresnel_sin_py = zeros(X.size);
for i, x in enumerate(X) :
    fresnel_sin_py[i] , fresnel_cos_py[i] = fresnel( x ) ;
    fresnel_cos_cpp[i] = Fresnel_Cosine_Integral(x) ;
    fresnel_sin_cpp[i] = Fresnel_Sine_Integral(x) ;
    
figure()
plot(X,fresnel_cos_py , label = "cos_py")
# plot(X,fresnel_sin_py , label = "sin_py")
plot(X,fresnel_cos_cpp , label = "cos_cpp")
# plot(X,fresnel_sin_cpp , label = "sin_cpp")
legend()
figure()
# plot(X,fresnel_cos_py , label = "cos_py")
plot(X,fresnel_sin_py , label = "sin_py")
# plot(X,fresnel_cos_cpp , label = "cos_cpp")
plot(X,fresnel_sin_cpp , label = "sin_cpp")
legend()

figure()
plot(X, absolute( fresnel_cos_py - fresnel_cos_cpp) , label = "cos_cpp")
plot(X, absolute( fresnel_sin_py - fresnel_sin_cpp) , label = "cos_cpp")


