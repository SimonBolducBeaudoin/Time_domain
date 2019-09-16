#include "../includes/fft_benchmarks_py.h"

// Macros to declare python classes (just to copy paste class declaration and change type )

		
#define PY_FFT_BENCHMARKS(FloatType,DataType)\
py::class_<FFT_Benchmarks<FloatType,DataType>>( m , "FFT_Benchmarks_"#FloatType"_"#DataType )\
		.def(py::init<uint64_t, uint32_t , int >(), py::arg("L_data")=0, py::arg("L_FFT")=0, py::arg("N_threads")=1)\
		.def("get_L_data", &FFT_Benchmarks<FloatType,DataType>::get_L_data)\
		.def_property("L_FFT", &FFT_Benchmarks<FloatType,DataType>::get_L_FFT, &FFT_Benchmarks<FloatType,DataType>::set_L_FFT)\
		.def_property("N_threads", &FFT_Benchmarks<FloatType,DataType>::get_nthreads, &FFT_Benchmarks<FloatType,DataType>::set_nthreads)\
		.def_readwrite("inplace_halfC", &FFT_Benchmarks<FloatType,DataType>::np_inplace_halfC)\
		.def_readwrite("direct", &FFT_Benchmarks<FloatType,DataType>::np_direct)\
		.def_readwrite("data", &FFT_Benchmarks<FloatType,DataType>::np_data)\
        .def_readwrite("time_alloc", &FFT_Benchmarks<FloatType,DataType>::Time_alloc)\
        .def_readwrite("time_execution", &FFT_Benchmarks<FloatType,DataType>::Time_execution)\
        .def_readwrite("time_preparation", &FFT_Benchmarks<FloatType,DataType>::Time_preparation)\
        .def("prepare" , &FFT_Benchmarks<FloatType,DataType>::prepare)\
		.def("execute" , &FFT_Benchmarks<FloatType,DataType>::execute)\
        ;
		
void init_fft_benchmarks(py::module &m)
{
	PY_FFT_BENCHMARKS(float,int16_t);
    PY_FFT_BENCHMARKS(double,int16_t);
}

// CLOSE MACRO SCOPES
#undef PY_FFT_BENCHMARKS
