#pragma once

#include "../../SM-Special_functions/special_functions.h"

#define _USE_MATH_DEFINES
#define PI M_PI
#include<cmath>
typedef unsigned int uint;
#include<complex>
typedef std::complex<double> complex_d;

/* 
	Assumes argument as length N and x_max = N - 1
*/
template <class FloatType>
void Tukey_Window( FloatType* argument , double alpha, uint N );

/*
	argument * Tukey(abscisse,alpha,x_max)
	argument and abscisse have length N
*/
template <class FloatType>
void Tukey_Window( FloatType* argument , double* abscisse, uint N , double alpha , double x_max );

/* 
	Assumes argument as length N and x_max = N - 1
*/
template <class FloatType>
void Tukey_modifed_Window( FloatType* argument , double x_1 , double x_2 , uint N );

/*
	argument * Tukey(abscisse,x_1,x_2,x_max)
	argument and abscisse have length N
*/
template <class FloatType>
void Tukey_modifed_Window( FloatType* argument , double* abscisse , uint N ,  double x_1 , double x_2 , double x_max );
