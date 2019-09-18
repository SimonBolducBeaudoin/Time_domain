//#define DEBUG
#include "../includes/TimeQuad.h"
#include "../src/TimeQuad.tpp"

// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019
template class TimeQuad<float,int16_t>;
template class TimeQuad<double,int16_t>;



 