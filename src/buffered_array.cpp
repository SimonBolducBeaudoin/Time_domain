#include "../includes/buffered_array.h"
#include"../src/buffered_array.tpp"
	
// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019
template class Buffered_Array<uint8_t>;
template class Buffered_Array<int8_t>;
template class Buffered_Array<uint16_t>;
template class Buffered_Array<int16_t>;
template class Buffered_Array<uint64_t>;
template class Buffered_Array<float>;
template class Buffered_Array<double>;
template class Buffered_Array<std::complex<float>>;
template class Buffered_Array<std::complex<double>>;

