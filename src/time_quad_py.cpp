#include "../includes/time_quad_py.h"
// Macros to declare python classes (just to copy paste class declaration and change type )

// DEBUG MACROS 
#ifdef DEBUG
	#define	PY_TIME_QUAD_DEBUG_FUNCS(FloatType,DataType) \
		.def_readwrite("FFT_kernel_p", &Time_Quad<FloatType, DataType>::np_FFT_kernel_p, "Return the FFT of p using FFTW (respecting the declared options and attributes).")\
		.def_readwrite("FFT_kernel_q", &Time_Quad<FloatType, DataType>::np_FFT_kernel_q, "Return the FFT of q using FFTW (respecting the declared options and attributes).")\
		.def("execute_kernel" , &Time_Quad<FloatType, DataType>::execute_kernel)\
		.def_readwrite("execute_time", &Time_Quad<FloatType, DataType>::execute_time)
#else
	#define	PY_TIME_QUAD_DEBUG_FUNCS(FloatType,DataType) //PLACE HOLDER
#endif	

// CLASS MACROS
#define PY_TIME_QUAD(FloatType,DataType)\
		py::class_<Time_Quad<FloatType, DataType>>( m , "Time_Quad_"#FloatType"_"#DataType )\
		.def(py::init<uint64_t, uint64_t, uint32_t, FloatType, std::string >(), py::arg("L_kernel")=2, py::arg("L_data")=16, py::arg("L_FFT")= 8, py::arg("dt")= 0.03125, py::arg("Convolution_type")="fft") \
		.def("get_L_kernel", &Time_Quad<FloatType,DataType>::get_L_kernel)\
		.def("get_L_data", &Time_Quad<FloatType, DataType>::get_L_data)\
		.def("get_dt", &Time_Quad<FloatType, DataType>::get_dt) \
		.def_property("L_FFT", &Time_Quad<FloatType, DataType>::get_L_FFT, &Time_Quad<FloatType, DataType>::set_L_FFT) \
		.def_readwrite("Convolution_type", &Time_Quad<FloatType, DataType>::Convolution_type) \
		.def_readwrite("p_full", &Time_Quad<FloatType, DataType>::np_p) \
		.def_readwrite("q_full", &Time_Quad<FloatType, DataType>::np_q) \
		.def_readwrite("p_valid", &Time_Quad<FloatType, DataType>::np_p_valid) \
		.def_readwrite("q_valid", &Time_Quad<FloatType, DataType>::np_q_valid) \
		.def_readwrite("kernel_p", &Time_Quad<FloatType, DataType>::np_kernel_p) \
		.def_readwrite("kernel_q", &Time_Quad<FloatType, DataType>::np_kernel_q) \
		.def_readwrite("data", &Time_Quad<FloatType, DataType>::np_data) \
		.def("print_all_attributes" , &Time_Quad<FloatType, DataType>::print_all_attributes, "Displays all possibly pertinent attributes (private or public).") \
		.def("execute" , &Time_Quad<FloatType, DataType>::execute, "Execute the convolution with declared options and attributes.")\
		PY_TIME_QUAD_DEBUG_FUNCS(FloatType,DataType)\
		\
		;

	
void init_time_quad(py::module &m)
{
	PY_TIME_QUAD(float,int16_t);
	PY_TIME_QUAD(double,int16_t);
}

// CLOSE MACRO SCOPES
#undef PY_TIME_QUAD
