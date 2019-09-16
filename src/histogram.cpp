
#include "../includes/histogram.h"
#include "../src/histogram_int.tpp"
#include "../src/histogram_float.tpp"

// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019

template class Histogram_of_floats<float,uint64_t>;
template class Histogram_of_floats<double,uint64_t>;

template class Histogram_of_ints<uint8_t,uint64_t>;
template class Histogram_of_ints<int8_t,uint64_t>;
template class Histogram_of_ints<uint16_t,uint64_t>;
template class Histogram_of_ints<int16_t,uint64_t>;

template class Histogram<float,uint64_t>;
template class Histogram<double,uint64_t>;
template class Histogram<uint8_t,uint64_t>;
template class Histogram<int8_t,uint64_t>;
template class Histogram<uint16_t,uint64_t>;
template class Histogram<int16_t,uint64_t>;


