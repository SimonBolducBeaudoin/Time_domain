#include "../includes/Windowing.h"

template <class FloatType>
void Tukey_Window( FloatType* argument , double alpha, uint N )
{
	double x_max = (double)(N-1) ;
	/*Could be parallelized*/
	for ( uint i = 0 ; i<N ; i++ )
	{
		argument[i] *= Tukey( (double)i , alpha , x_max );
	}
}

template <class FloatType>
void Tukey_Window( FloatType* argument , double* abscisse, uint N , double alpha , double x_max )
{
	/*Could be parallelized*/
	for ( uint i = 0 ; i<N ; i++ )
	{
		argument[i] *= Tukey( abscisse[i] , alpha , x_max );
	}
}

template <class FloatType>
void Tukey_modifed_Window( FloatType* argument , double x_1 , double x_2 , uint N )
{
	double x_max = (double)(N-1) ;
	/*Could be parallelized*/
	for ( uint i = 0 ; i<N ; i++ )
	{
		argument[i] *= Tukey_modifed( (double)i , x_1 , x_2 , x_max );
	}
}

template <class FloatType>
void Tukey_modifed_Window( FloatType* argument , double* abscisse , uint N , double x_1 , double x_2 , double x_max )
{
	/*Could be parallelized*/
	for ( uint i = 0 ; i<N ; i++ )
	{
		argument[i] *= Tukey_modifed( abscisse[i] , x_1 , x_2 , x_max );
	}
}

