#include "../includes/Windowing.h"

template <class FloatType>
void Tukey_Window( FloatType* Y , double alpha, uint N)
{
	// Y : data to applie the window on
	// alpha : the %age of Y that stays unchanged
	// N The length of the array associated with Y

	for (uint i = 0; i < (uint)(alpha*N/2.0); i++ )
	{
		Y[i] *= (0.5)*( 1 + cos( 2.0*PI*i/(alpha*N) - PI )  ) ;
	}
	
	for (uint i = ceil(N * (1-alpha/2.0) ) ; i < N; i++ )
	{
		Y[i] *= (0.5)*( 1 + cos( 2.0*PI*i/(alpha*N) - 2.0*PI/alpha + PI ) ) ;
	}
};

template <>
void Tukey_Window<complex_d>( complex_d* Y , double alpha, uint N)
{
	// specialization of the Tukey_Window function that is intended for used on half-complex data 
	// (i.e. the output of a r2c fft)
	// Y : data to applie the window on (of length N/2 +1)
	// alpha : the %age of Y that stays unchanged
	// N The length of the real array (before the fft)
	
	for (uint i = ceil( N*0.5*(1.0-alpha) )  ; i < ceil(N/2.0); i++ )
	{
		Y[i] *= (0.5)*( 1 + cos( 2.0*PI*i/(alpha*N) - PI/alpha + PI ) ) ;
	}
};

