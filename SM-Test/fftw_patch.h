#pragma once

#include "../../includes/header_common.h"

// FFTW_MALLOC
template<DataType>
void* FFTW_MALLOC(DataType, size_t n);

template<>
void* FFTW_MALLOC(double, size_t n)
{
	return fftw_malloc( sizeof(double)*n );
}

// FFTW_FREE
template<DataType>
void* FFTW_MALLOC(DataType, size_t n);

template<PlanType,FloatType,FFTW_Complex_Type>
PlanType FFTW_PLAN_DFT_R2C(int n, FloatType* in, FFTW_Complex_Type* out , unsigned flag);

template<>
fftw_plan FFTW_PLAN_DFT_R2C(int n, double* in, fftw_complex* out , unsigned flag)
{
	return fftw_plan_dft_r2c_1d(int n, double *in, fftw_complex *out, unsigned flags);
}


