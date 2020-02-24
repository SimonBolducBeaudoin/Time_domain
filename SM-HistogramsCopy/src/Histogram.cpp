#include "../includes/histogram.h"

// Methods tempalte definitions
#include "../src/histogram.tpp"
// Methods template intanciation


// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019

#define HISTOGRAM(BinType)\
template class Histogram<BinType>;\
template void Histogram<BinType>::accumulate_float_py( py::array_t<double> data, double max ) ;\
template void Histogram<BinType>::accumulate_float_py( py::array_t<float> data, float max ) ;\
\
template void Histogram<BinType>::float_to_hist(double data, BinType* histogram , double max , double bin_width );\
template void Histogram<BinType>::float_to_hist(float data, BinType* histogram , float max , float bin_width );


HISTOGRAM(uint64_t);
HISTOGRAM(uint32_t);
HISTOGRAM(uint16_t);
HISTOGRAM(uint8_t);


#undef PY_HISTOGRAM
