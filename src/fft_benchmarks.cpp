
#include "../includes/fft_benchmarks.h"
#include "../src/fft_benchmarks.tpp"

//  Note that fft_test.ttp and  fft_test.h will probably need to be modified if 
// you want to add more type compatibility

// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019

template class Mother_FFT<float,int16_t>;
template class Mother_FFT<double,int16_t>;
template class FFT_Benchmarks<float,int16_t>;
template class FFT_Benchmarks<double,int16_t>;



 