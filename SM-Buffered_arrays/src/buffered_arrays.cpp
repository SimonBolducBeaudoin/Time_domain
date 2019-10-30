#include "../includes/buffered_arrays.h"
#include"../src/buffered_arrays.tpp"
	
// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019
template class Buffered_Vector<uint8_t>;
template class Buffered_Vector<int8_t>;
template class Buffered_Vector<uint16_t>;
template class Buffered_Vector<int16_t>;
template class Buffered_Vector<uint64_t>;
template class Buffered_Vector<float>;
template class Buffered_Vector<double>;
template class Buffered_Vector<std::complex<float>>;
template class Buffered_Vector<std::complex<double>>;

