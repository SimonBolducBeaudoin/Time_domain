#include "fftw_patch.h"
#include "../../includes/header_common.h"

float fftwf(float X)
{
	printf("I just used the float version.");
	return X
}

double fftwf(double X)
{
	printf("I just used the double version.");
	return X
}

template<>
float FFTW (float X)
{
	return fftwf(X)
}

template<>
float FFTW (float X)
{
	return fftwf(X)
}

