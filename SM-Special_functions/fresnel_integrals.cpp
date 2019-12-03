#include "fresnel_integrals.h"

inline double C( double x )
{
	return cos((PI/2.0)*x*x) ;
}

inline double S( double x )
{
	return sin((PI/2.0)*x*x) ;
}

double Fresnel_Cosine_Integral( double x )
{
	return Newton_Cotes_11( &C , 0 , x, 1000 );
}

double Fresnel_Sine_Integral( double x )
{
	return Newton_Cotes_11( &S , 0 , x , 1000 );
}






