#pragma once

#define _USE_MATH_DEFINES
#define PI M_PI
#include<math.h>
typedef unsigned int uint;
#include<complex>
typedef std::complex<double> complex_d;

template <class FloatType>
void Tukey_Window( FloatType* Y , double alpha, uint N );

