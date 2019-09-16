#include "../includes/histograms_py.h"

// Macros to declare python classes (just to copy paste class declaration and change type )

// CLASS MACROS		
#define PY_HISTOGRAM_FLOATS(FloatType,BinType)\
py::class_<Histogram<FloatType,BinType>>( m , "Histogram_"#FloatType"_"#BinType )\
		.def(py::init< 	uint64_t , py::array_t<FloatType> , FloatType , std::string , int >(), py::arg("NofBins")=0, py::arg("Data")=0 , py::arg("Max") = 1 , py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def(py::init< 	py::array_t<FloatType> , py::array_t<FloatType> , FloatType , std::string , int >(), py::arg("Hist")=0 , py::arg("Data")=0 , py::arg("Max") = 1 , py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def_readwrite("Algorithm", &Histogram<FloatType,BinType>::Algorithme) \
		.def_readwrite("Hist", &Histogram<FloatType,BinType>::np_histogram) \
		.def_property("Max", &Histogram<FloatType,BinType>::get_max, &Histogram<FloatType,BinType>::set_max)\
		.def_property("N_threads", &Histogram<FloatType,BinType>::get_n_threads, &Histogram<FloatType,BinType>::set_n_threads)\
		;

#define PY_HISTOGRAM_INTS(IntType,BinType)\
py::class_<Histogram<IntType,BinType>>( m , "Histogram_"#IntType"_"#BinType )\
		.def(py::init< 	uint64_t , py::array_t<IntType> , std::string , int >(), py::arg("NofBins")=0, py::arg("Data")=0 , py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def(py::init< 	py::array_t<IntType> , py::array_t<IntType> , std::string , int >(), py::arg("Hist")=0 , py::arg("Data")=0 , py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def_readwrite("Algorithm", &Histogram<IntType,BinType>::Algorithme) \
		.def_readwrite("Hist", &Histogram<IntType,BinType>::np_histogram) \
		.def_property("N_threads", &Histogram<IntType,BinType>::get_n_threads, &Histogram<IntType,BinType>::set_n_threads)\
		;
	
#define PY_HISTOGRAM2D_FLOATS(FloatType,BinType)\
py::class_<Histogram2D<FloatType,BinType>>( m , "Histogram2D_"#FloatType"_"#BinType )\
		.def(py::init< 	uint64_t , py::array_t<FloatType> , py::array_t<FloatType> , FloatType , std::string , int >(), py::arg("NofBins")=0, py::arg("Data")=0 , py::arg("Max") = 1 , py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def(py::init< 	py::array_t<FloatType> , py::array_t<FloatType> , FloatType , std::string , int >(), py::arg("Hist")=0 , py::arg("Data")=0 , py::arg("Max") = 1 , py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def_readwrite("Algorithm", &Histogram2D<FloatType,BinType>::Algorithme) \
		.def_readwrite("Hist", &Histogram2D<FloatType,BinType>::np_histogram) \
		.def_property("Max", &Histogram2D<FloatType,BinType>::get_max, &Histogram2D<FloatType,BinType>::set_max)\
		.def_property("N_threads", &Histogram2D<FloatType,BinType>::get_n_threads, &Histogram2D<FloatType,BinType>::set_n_threads)\
		;	
		
#define PY_HISTOGRAM2D_INTS(IntType,BinType)\
py::class_<Histogram2D<IntType,BinType>>( m , "Histogram2D_"#IntType"_"#BinType )\
		.def(py::init< 	uint64_t , py::array_t<IntType> , py::array_t<IntType> , std::string , int >(), py::arg("NofBins")=0, py::arg("Data")=0, py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def(py::init< 	py::array_t<IntType> , py::array_t<IntType> , std::string , int >(), py::arg("Hist")=0 , py::arg("Data")=0 , py::arg("Algorithm") = "", py::arg("N_threads") = 4 )\
		.def_readwrite("Algorithm", &Histogram2D<IntType,BinType>::Algorithme) \
		.def_readwrite("Hist", &Histogram2D<IntType,BinType>::np_histogram) \
		.def_property("N_threads", &Histogram2D<IntType,BinType>::get_n_threads, &Histogram2D<IntType,BinType>::set_n_threads)\
		;	

void init_histograms(py::module &m)
{
	PY_HISTOGRAM_FLOATS(float,uint64_t);
	PY_HISTOGRAM_FLOATS(double,uint64_t);
	PY_HISTOGRAM_INTS(uint8_t,uint64_t);
	PY_HISTOGRAM_INTS(int8_t,uint64_t);
	PY_HISTOGRAM_INTS(uint16_t,uint64_t);
	PY_HISTOGRAM_INTS(int16_t,uint64_t);
}

// CLOSE MACRO SCOPES
#undef PY_HISTOGRAM2D_FLOATS
#undef PY_HISTOGRAM2D_INTS
#undef PY_HISTOGRAM_FLOATS
#undef PY_HISTOGRAM_INTS

