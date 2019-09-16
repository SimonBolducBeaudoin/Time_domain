
#include "../includes/histogram2D.h"
#include "../src/histogram2D_float.tpp"
#include "../src/histogram2D_int.tpp"

// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019

template class Histogram2D_of_floats<float,uint64_t>;
template class Histogram2D_of_floats<double,uint64_t>;

template class Histogram2D_of_ints<int8_t,uint64_t>;
template class Histogram2D_of_ints<int16_t,uint64_t>;

template class Histogram2D<float,uint64_t>;
template class Histogram2D<double,uint64_t>;
template class Histogram2D<uint8_t,uint64_t>;
template class Histogram2D<int8_t,uint64_t>;
template class Histogram2D<uint16_t,uint64_t>;
template class Histogram2D<int16_t,uint64_t>;