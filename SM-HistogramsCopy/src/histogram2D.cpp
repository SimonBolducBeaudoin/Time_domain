
#include "../includes/histogram2D.h"

// Methods template definitions
#include "../src/histogram2D.tpp"


// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019

#define HISTOGRAM2D(BinType)\
template class Histogram2D<BinType>;\
template void Histogram2D<BinType>::accumulate_float_py( py::array_t<double> data_1, py::array_t<double> data_2, double max ) ;\
template void Histogram2D<BinType>::accumulate_float_py( py::array_t<float> data_1, py::array_t<float> data_2, float max ) ;\
\
template void Histogram2D<BinType>::what_bin_float( double data_1 , double data_2, double max, double bin_width, uint16_t* binx, uint16_t* biny ) ;\
template void Histogram2D<BinType>::what_bin_float( float data_1 , float data_2, float max, float bin_width, uint16_t* binx, uint16_t* biny ) ;

HISTOGRAM2D(uint64_t);
HISTOGRAM2D(uint32_t);
HISTOGRAM2D(uint16_t);

#undef PY_HISTOGRAM2D

