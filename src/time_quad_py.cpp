#include "../includes/TimeQuad_py.h"

// CLASS MACROS
#define PY_TIME_QUAD(FloatType,DataType)\
		py::class_<TimeQuad<FloatType, DataType>>( m , "TimeQuad_"#FloatType"_"#DataType )\
		.def(py::init<uint64_t, uint64_t, uint32_t, FloatType, std::string >(), py::arg("L_kernel")=2, py::arg("L_data")=16, py::arg("L_FFT")= 8, py::arg("dt")= 0.03125, py::arg("Convolution_type")="fft") \
		.def("get_L_kernel", &TimeQuad<FloatType,DataType>::get_L_kernel)\
		.def("get_L_data", &TimeQuad<FloatType, DataType>::get_L_data)\
		.def("get_dt", &TimeQuad<FloatType, DataType>::get_dt) \
		.def_property("L_FFT", &TimeQuad<FloatType, DataType>::get_L_FFT, &TimeQuad<FloatType, DataType>::set_L_FFT) \
		.def_readwrite("Convolution_type", &TimeQuad<FloatType, DataType>::Convolution_type) \
		.def_readwrite("p_full", &TimeQuad<FloatType, DataType>::np_p) \
		.def_readwrite("q_full", &TimeQuad<FloatType, DataType>::np_q) \
		.def_readwrite("p_valid", &TimeQuad<FloatType, DataType>::np_p_valid) \
		.def_readwrite("q_valid", &TimeQuad<FloatType, DataType>::np_q_valid) \
		.def_readwrite("kernel_p", &TimeQuad<FloatType, DataType>::np_kernel_p) \
		.def_readwrite("kernel_q", &TimeQuad<FloatType, DataType>::np_kernel_q) \
		.def_readwrite("data", &TimeQuad<FloatType, DataType>::np_data) \
		.def("print_all_attributes" , &TimeQuad<FloatType, DataType>::print_all_attributes, "Displays all possibly pertinent attributes (private or public).") \
		.def("execute" , &TimeQuad<FloatType, DataType>::execute, "Execute the convolution with declared options and attributes.")\
		\
		;

	
void init_TimeQuad(py::module &m)
{
	PY_TIME_QUAD(float,int16_t);
	PY_TIME_QUAD(double,int16_t);
}

// CLOSE MACRO SCOPES
#undef PY_TIME_QUAD
