#include "../includes/FastConvolution.h"
#include "../src/FastConvolution.tpp"

// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019
template class Mother_FastConvolution<int16_t,int16_t,int16_t,complex_f>;
template class Mother_FastConvolution<int16_t,int16_t,int16_t,complex_d>;
template class Mother_FastConvolution<float,float,int16_t,complex_f>;
template class Mother_FastConvolution<double,double,int16_t,complex_d>;

template class FastConvolution<int16_t,int16_t,int16_t,complex_f>;
template class FastConvolution<int16_t,int16_t,int16_t,complex_d>;
template class FastConvolution<float,float,int16_t,complex_f>;
template class FastConvolution<double,double,int16_t,complex_d>;

//test