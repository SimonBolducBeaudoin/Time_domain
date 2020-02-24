#include "../includes/histogram_py.h"

// CLASS MACROS		
#define PY_HISTOGRAM(BinType)\
py::class_<Histogram<BinType>>( m , "Histogram_"#BinType )\
	.def(py::init< 	uint64_t , BinType >(), "nofbins"_a.noconvert() = 0,  "n_threads"_a.noconvert() = 2 )\
	.def("accumulate", &Histogram<BinType>::accumulate_float_py<double>, "data"_a.noconvert() = py::array_t<double>(), "max"_a = (double)(1) )\
	.def("accumulate", &Histogram<BinType>::accumulate_float_py<float>, "data"_a.noconvert() = py::array_t<float>(), "max"_a = (float)(1) )\
	.def("accumulate", (void (Histogram<BinType>::*)(py::array_t<uint8_t>) )  &Histogram<BinType>::accumulate_int_py, "data"_a.noconvert() = py::array_t<uint8_t>() )\
	.def("accumulate", (void (Histogram<BinType>::*)(py::array_t<uint16_t> , int) ) &Histogram<BinType>::accumulate_int_py, "data"_a.noconvert() = py::array_t<uint16_t>(), "b"_a = 16 )\
	.def("get", &Histogram<BinType>::get_py)\
	.def("abscisse", &Histogram<BinType>::abscisse_float_py, "max"_a.noconvert())\
	.def("moment_k", &Histogram<BinType>::moment_k , "k"_a = 1 , "bin_centre"_a = 0 , "bin_width"_a = 1.0 )\
	.def("moment_k_float", &Histogram<BinType>::moment_k_float , "k"_a = 1 , "max"_a = 1.0 )\
	.def("centered_moment_k", &Histogram<BinType>::centered_moment_k , "k"_a = 2 , "bin_centre"_a = 0 , "bin_width"_a = 1.0 )\
	.def("centered_moment_k_float", &Histogram<BinType>::centered_moment_k_float , "k"_a = 2 , "max"_a = 1.0 )\
	.def("cumulant_k", &Histogram<BinType>::cumulant_k , "k"_a = 2 , "bin_centre"_a = 0 , "bin_width"_a = 1.0 )\
	.def("cumulant_k_float", &Histogram<BinType>::cumulant_k_float , "k"_a = 2 , "max"_a = 1.0 )\
	;
       

#define PY_HISTOGRAM2D(BinType)\
py::class_<Histogram2D<BinType>>( m , "Histogram2D_"#BinType )\
	.def(py::init< 	uint64_t , BinType >(), "nofbins"_a.noconvert() = 0,  "n_threads"_a.noconvert() = 2 )\
	.def("accumulate", &Histogram2D<BinType>::accumulate_float_py<double>, "data_1"_a.noconvert() = py::array_t<double>(), "data_2"_a.noconvert() = py::array_t<double>(), "max"_a = (double)(1) )\
	.def("accumulate", &Histogram2D<BinType>::accumulate_float_py<float>, "data_1"_a.noconvert() = py::array_t<float>(), "data_2"_a.noconvert() = py::array_t<float>(), "max"_a = (float)(1) )\
	.def("accumulate", (void (Histogram2D<BinType>::*)(py::array_t<uint8_t>, py::array_t<uint8_t>) ) &Histogram2D<BinType>::accumulate_int_py, "data_1"_a.noconvert() = py::array_t<uint8_t>(), "data_2"_a.noconvert() = py::array_t<uint8_t>() )\
	.def("accumulate", (void (Histogram2D<BinType>::*)(py::array_t<uint16_t> , py::array_t<uint16_t> , int) ) &Histogram2D<BinType>::accumulate_int_py, "data_1"_a.noconvert() = py::array_t<uint16_t>(), "data_2"_a.noconvert() = py::array_t<uint16_t>(), "b"_a = 16 )\
	.def("get", &Histogram2D<BinType>::get_py)\
	.def("abscisse", &Histogram2D<BinType>::abscisse_float_py, "max"_a.noconvert())\
	/*.def_readwrite("accumulate_timer", &Histogram2D<BinType>::accumulate_timer)*/\
	.def("moment2D", &Histogram2D<BinType>::moment2D , "exp_x"_a = 0 , "exp_y"_a = 0 , "bin_centre"_a = 0 , "bin_width"_a = 1.0 )\
	.def("moment2D_float", &Histogram2D<BinType>::moment2D_float ,  "exp_x"_a = 0 , "exp_y"_a = 0, "max"_a = 1.0 )\
	.def("cumulant_ii", &Histogram2D<BinType>::cumulant_ii , "bin_centre"_a = 0 , "bin_width"_a = 1.0 )\
	.def("cumulant_ii_float", &Histogram2D<BinType>::cumulant_ii_float , "max"_a = 1.0 )\
	.def("cumulant_jj", &Histogram2D<BinType>::cumulant_jj , "bin_centre"_a = 0 , "bin_width"_a = 1.0 )\
	.def("cumulant_jj_float", &Histogram2D<BinType>::cumulant_jj_float , "max"_a = 1.0 )\
	.def("cumulant_iijj", &Histogram2D<BinType>::cumulant_iijj , "bin_centre"_a = 0 , "bin_width"_a = 1.0 )\
	.def("cumulant_iijj_float", &Histogram2D<BinType>::cumulant_iijj_float , "max"_a = 1.0 )\
	;

void init_Histograms(py::module &m)
{
	PY_HISTOGRAM(uint64_t);
	PY_HISTOGRAM(uint32_t);
	PY_HISTOGRAM(uint16_t);
	PY_HISTOGRAM(uint8_t);
	
	PY_HISTOGRAM2D(uint64_t);
	PY_HISTOGRAM2D(uint32_t);
	PY_HISTOGRAM2D(uint16_t);
	
}

// CLOSE MACRO SCOPES
#undef PY_HISTOGRAM
#undef PY_HISTOGRAM2D

