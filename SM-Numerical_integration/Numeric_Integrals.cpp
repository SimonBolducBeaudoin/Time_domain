#include "Numeric_Integrals.h"

double Newton_Cotes_11 ( double (*Function)(double), double lower_bound , double upper_bound , uint N_steps )
{
	double h = (upper_bound-lower_bound)/(10*N_steps);
	double I_1 = 0 ;
	double I_2 = 0 ;
	double I_3 = 0 ;
	double I_4	= 0 ;
	double I_5	= 0 ;
	double I_6	= 0 ;
	
	double prefact = 5.0/299376.0;
	
	double k1 = 16067;
	double k2 = 106300;
	double k3 = -48525;
	double k4 = 272400 ;
	double k5 = -260550;
	double k6 = 427368;
	
	double x = lower_bound ; 
	
	for( uint i = 0 ; i < N_steps ; i++)
	{
		I_1 += Function( x       ) + Function( x + 10*h ) ;
		I_2 += Function( x + h   ) + Function( x + 9 *h ) ;
		I_3 += Function( x + 2*h ) + Function( x + 8 *h ) ;
		I_4 += Function( x + 3*h ) + Function( x + 7 *h ) ;
		I_5 += Function( x + 4*h ) + Function( x + 6 *h ) ;
		I_6 += Function( x + 5*h ) ;
		x += 10*h ; 
	}
	
	return prefact*h*( k1*I_1 + k2*I_2 + k3*I_3 + k4*I_4 + k5*I_5 + k6*I_6 );
}