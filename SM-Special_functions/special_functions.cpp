#include "special_functions.h"


/* Fresnel integrals */
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

/* Windowing functions */
double Tukey( double x , double alpha , double x_max )
{
	/*
	symetric ( even function ) and cyclic
	*/
	double L = x_max + 1 ;
	double X_alpha = alpha*L/2.0 ;
    double X = fmod(fabs(x),x_max) ;
    if ( (0<=X) && (X < X_alpha) )
	{
		return 0.5 - 0.5*cos( PI*X/X_alpha ) ;
	}
    else if ( x_max-X_alpha<=X && X < x_max )
	{
		return 0.5 - 0.5*cos( PI*(x_max-X)/X_alpha );
	} 
    else
	{
		return 1.0 ;
	}  
}

double Tukey_modifed( double x , double x_1 , double x_2 , double x_max)
{
	/*
	symetric ( even function ) and cyclic
	1st transition stops at x_1
    2nd transition starts at x_2
    The function's period is x_max
	*/
    double X = fabs( fmod(x,x_max) ) ;
    if ( (0<=X) && (X < x_1) )
	{
		return 0.5 - 0.5*cos( PI*X/x_1 );
	}
	else if ( (x_2<=X) && (X < x_max) )
	{
		return 0.5 - 0.5*cos( PI*(x_max-X)/(x_max-x_2) );
	}
    else
	{
		return 1.0 ;
	}  
}
	



