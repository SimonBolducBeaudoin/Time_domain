#include "../includes/FastConvolution_py.h"
// Macros to declare python classes (just to copy paste class declaration and change type )


// CLASS MACROS
#define PY_FAST_CONVOLUTION(OutputType, KernelType, DataType, ComplexType)\
py::class_<FastConvolution<OutputType, KernelType, DataType, ComplexType>>( m , "FastConvolution_"#OutputType"_"#KernelType"_"#DataType"_"#ComplexType )\
	.def(py::init<uint64_t, uint64_t, uint32_t, std::string , int>(), py::arg("l_kernel") = 1<<8 , py::arg("l_data")=1<<26, py::arg("l_FFT")= 1<<10, py::arg("convolution_type")="fft" , py::arg("n_threads") = 70) \
	.def("get_l_kernel", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::get_L_kernel)\
	.def("get_l_data", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::get_L_data)\
	.def_property("l_fft", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::get_L_FFT, &FastConvolution<OutputType,KernelType, DataType, ComplexType>::set_L_FFT) \
	.def_property("n_threads", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::get_n_threads, &FastConvolution<OutputType,KernelType, DataType, ComplexType>::set_n_threads) \
	.def_readwrite("convolution_type", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::Convolution_type) \
    .def_readwrite("data", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::np_data) \
    .def_readwrite("kernel", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::np_kernel) \
	.def_readwrite("output_full", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::np_output) \
	.def("execute" , &FastConvolution<OutputType,KernelType, DataType, ComplexType>::execute, "Execute the convolution with declared options and attributes.")\
    .def_readwrite("time_execution", &FastConvolution<OutputType,KernelType, DataType, ComplexType>::Time_execution)\
	\
	;

void init_FastConvolution(py::module &m)
{
	PY_FAST_CONVOLUTION(int16_t,int16_t,int16_t,complex_f);
	PY_FAST_CONVOLUTION(int16_t,int16_t,int16_t,complex_d);
	PY_FAST_CONVOLUTION(float,float,int16_t,complex_f);
	PY_FAST_CONVOLUTION(double,double,int16_t,complex_d);
}

// CLOSE MACRO SCOPES
#undef PY_FAST_CONVOLUTION



