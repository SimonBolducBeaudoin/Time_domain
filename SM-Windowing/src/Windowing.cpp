#include "../includes/Windowing.h"
#include "../src/Windowing.tpp"

// Explicit template instanciation
template void Tukey_Window<double>( double* Y , double alpha , uint N );
template void Tukey_Window<complex_d>( complex_d* Y , double alpha , uint N );

template void Tukey_Window<double>( double* argument , double* abscisse, uint N , double alpha , double x_max );
template void Tukey_Window<complex_d>( complex_d* argument , double* abscisse, uint N , double alpha , double x_max );

template void Tukey_modifed_Window<double>( double* Y , double x_1 , double x_2 , uint N );
template void Tukey_modifed_Window<complex_d>( complex_d* Y , double x_1 , double x_2 , uint N );

template void Tukey_modifed_Window<double>( double* argument , double* abscisse, uint N , double x_1 , double x_2 , double x_max );
template void Tukey_modifed_Window<complex_d>( complex_d* argument , double* abscisse, uint N , double x_1 , double x_2 , double x_max );




 
