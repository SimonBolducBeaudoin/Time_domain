#include "../includes/fast_convolution_py.h"
// Macros to declare python classes (just to copy paste class declaration and change type )


// CLASS MACROS
#define PY_FAST_CONVOLUTION(OutputType, KernelType, DataType, ComplexType)\
py::class_<Fast_Conv<OutputType, KernelType, DataType, ComplexType>>( m , "Fast_Conv_"#OutputType"_"#KernelType"_"#DataType"_"#ComplexType )\
	.def(py::init<uint64_t, uint64_t, uint32_t, std::string , int>(), py::arg("L_kernel") = 2, py::arg("L_data")=16, py::arg("L_FFT")= 8, py::arg("Convolution_type")="fft" , py::arg("N_threads")) \
	.def("get_L_kernel", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::get_L_kernel)\
	.def("get_L_data", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::get_L_data)\
	.def_property("L_FFT", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::get_L_FFT, &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::set_L_FFT) \
	.def_property("N_threads", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::get_n_threads, &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::set_n_threads) \
	.def_readwrite("Convolution_type", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::Convolution_type) \
	.def_readwrite("Output_full", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::np_output) \
	.def_readwrite("Output_valid", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::np_output_valid) \
	.def_readwrite("Data", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::np_data) \
	.def_readwrite("Kernel", &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::np_kernel) \
	.def("print_all_attributes" , &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::print_all_attributes, "Displays all possibly pertinent attributes (private or public).") \
	.def("prepare" , &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::prepare, "Must be declared once before excute().")\
	.def("execute" , &Fast_Conv<OutputType,KernelType, DataType, ComplexType>::execute, "Execute the convolution with declared options and attributes.")\
	\
	;

void init_fast_convolution(py::module &m)
{
	PY_FAST_CONVOLUTION(int16_t,int16_t,int16_t,complex_f);
	PY_FAST_CONVOLUTION(int16_t,int16_t,int16_t,complex_d);
	PY_FAST_CONVOLUTION(float,float,int16_t,complex_f);
	PY_FAST_CONVOLUTION(double,double,int16_t,complex_d);
}

// CLOSE MACRO SCOPES
#undef PY_FAST_CONVOLUTION



