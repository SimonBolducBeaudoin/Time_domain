#include "../includes/Windowing.h"
#include "../src/Windowing.tpp"

// Explicit template instanciation
template void Tukey_Window<double>( double* Y , double alpha , uint N);
template void Tukey_Window<complex_d>( complex_d* Y , double alpha , uint N);




 
