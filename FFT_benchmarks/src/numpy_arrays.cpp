#include "../includes/numpy_arrays.h"
#include"../src/numpy_arrays.tpp"
	
// Explicit template instanciation
// See: https://docs.microsoft.com/en-us/cpp/cpp/explicit-instantiation?view=vs-2019
template class Numpy_Vector<uint8_t>;
// template class Numpy_Vector<int8_t>;
// template class Numpy_Vector<uint16_t>;
// template class Numpy_Vector<int16_t>;
// template class Numpy_Vector<uint64_t>;
// template class Numpy_Vector<float>;
// template class Numpy_Vector<double>;
// template class Numpy_Vector<std::complex<float>>;
// template class Numpy_Vector<std::complex<double>>;

