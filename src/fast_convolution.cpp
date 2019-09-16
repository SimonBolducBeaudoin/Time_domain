#include "../includes/fast_convolution.h"
#include "../src/fast_convolution.tpp"

// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019
template class Mother_Fast_Conv<int16_t,int16_t,int16_t,complex_f>;
template class Mother_Fast_Conv<int16_t,int16_t,int16_t,complex_d>;
template class Mother_Fast_Conv<float,float,int16_t,complex_f>;
template class Mother_Fast_Conv<double,double,int16_t,complex_d>;

template class Fast_Conv<int16_t,int16_t,int16_t,complex_f>;
template class Fast_Conv<int16_t,int16_t,int16_t,complex_d>;
template class Fast_Conv<float,float,int16_t,complex_f>;
template class Fast_Conv<double,double,int16_t,complex_d>;

//test