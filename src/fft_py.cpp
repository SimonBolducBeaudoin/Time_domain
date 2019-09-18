#include "../includes/fft_py.h"

// Macros to declare python classes (just to copy paste class declaration and change type )
#define PY_FFT(FloatType,DataType)\
py::class_<FFT<FloatType,DataType>>( m , "FFT_"#FloatType"_"#DataType )\
		.def(py::init<uint64_t, uint32_t , int >(), py::arg("L_data")=0, py::arg("L_FFT")=0, py::arg("N_threads")=1)\
		.def("get_L_data", &FFT<FloatType,DataType>::get_L_data)\
		.def_property("L_FFT", &FFT<FloatType,DataType>::get_L_FFT, &FFT<FloatType,DataType>::set_L_FFT)\
		.def_property("N_threads", &FFT<FloatType,DataType>::get_nthreads, &FFT<FloatType,DataType>::set_nthreads)\
		.def_readwrite("inplace_halfC", &FFT<FloatType,DataType>::np_inplace_halfC)\
		.def_readwrite("direct", &FFT<FloatType,DataType>::np_direct)\
		.def_readwrite("data", &FFT<FloatType,DataType>::np_data)\
        .def_readwrite("time_alloc", &FFT<FloatType,DataType>::Time_alloc)\
        .def_readwrite("time_execution", &FFT<FloatType,DataType>::Time_execution)\
        .def_readwrite("time_preparation", &FFT<FloatType,DataType>::Time_preparation)\
        .def("prepare" , &FFT<FloatType,DataType>::prepare)\
		.def("execute" , &FFT<FloatType,DataType>::execute)\
        ;
		
void init_fft(py::module &m)
{
	PY_FFT(float,int16_t);
    PY_FFT(double,int16_t);
}

// CLOSE MACRO SCOPES
#undef PY_FFT
