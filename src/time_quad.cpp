//#define DEBUG
#include "../includes/time_quad.h"
#include "../src/time_quad.tpp"

// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019
template class Time_Quad<float,int16_t>;
template class Time_Quad<double,int16_t>;



 