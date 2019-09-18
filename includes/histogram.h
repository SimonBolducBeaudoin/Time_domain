#pragma once

#include "../includes/omp_extra.h"
#include "../includes/Histogram_int.h"
#include "../includes/Histogram_float.h"

//////////////////////////////////////////////////////////////////////////////////////
//// With partial specialization all implementation will appears under the same class name "Histogram"
// Histogram_1D primary template
template <class IntType, class BinType>
class Histogram
{
	// This is only to make the patern Histogram<IntType,BinType> 
	// available in the namespace.
};

// partial specializations -------->>>>>>>>

// FLOAT AND DOUBLE //////////////////////////////////////////////

///////////////////////////////////
// BEGINING OF MACROS SCOPE
#define DECLARE_INPUTS(FloatType)\
	uint64_t NofBins , FloatType* data , uint64_t L_data , FloatType max , std::string Algorithme , int n_threads
	
#define INPUTS \
	NofBins , data , L_data , max , Algorithme , n_threads

#define DECLARE_INPUTS_PY_1(FloatType)\
	uint64_t NofBins, py::array_t<FloatType> numpy_data , FloatType max , std::string Algorithme , int n_threads
	
#define INPUTS_PY_1 \
	NofBins , numpy_data , max , Algorithme , n_threads
	
#define DECLARE_INPUTS_PY_2(FloatType)\
	py::array_t<FloatType> numpy_hist, py::array_t<FloatType> numpy_data , FloatType max , std::string Algorithme , int n_threads
	
#define INPUTS_PY_2 \
	numpy_hist , numpy_data , max , Algorithme , n_threads
///////////////////////////////////

template <class BinType>
class Histogram<float,BinType> : public Histogram_of_floats<float,BinType>
{
public : 
	Histogram( DECLARE_INPUTS(float) ): Histogram_of_floats<float,BinType>( INPUTS ) {}
	Histogram( DECLARE_INPUTS_PY_1(float) ): Histogram_of_floats<float,BinType>( INPUTS_PY_1 ) {}
	Histogram( DECLARE_INPUTS_PY_2(float) ): Histogram_of_floats<float,BinType>( INPUTS_PY_2 ) {}
	
	void execute();
};

template <class BinType>
class Histogram<double,BinType> : public Histogram_of_floats<double,BinType>
{
public : 
	Histogram( DECLARE_INPUTS(double) ): Histogram_of_floats<double,BinType>( INPUTS ) {}
	Histogram( DECLARE_INPUTS_PY_1(double) ): Histogram_of_floats<double,BinType>( INPUTS_PY_1 ) {}
	Histogram( DECLARE_INPUTS_PY_2(double) ): Histogram_of_floats<double,BinType>( INPUTS_PY_2 ) {}
	
	void execute();
};

///////////////////////////////////
// END of MACROS SCOPES
#undef DECLARE_INPUTS
#undef INPUTS 
#undef DECLARE_INPUTS_PY_1
#undef INPUTS_PY_1
#undef DECLARE_INPUTS_PY_2
#undef INPUTS_PY_2
///////////////////////////////////


// UINT8 AND INT8 //////////////////////////////////////////////
///////////////////////////////////
// BEGINING OF MACROS SCOPE
#define DECLARE_INPUTS(IntType)\
	uint64_t NofBins , IntType* data , uint64_t L_data, std::string Algorithme , int n_threads
	
#define INPUTS \
	NofBins , data , L_data , Algorithme , n_threads

#define DECLARE_INPUTS_PY_1(IntType)\
	uint64_t NofBins, py::array_t<IntType> numpy_data , std::string Algorithme , int n_threads
	
#define INPUTS_PY_1 \
	NofBins , numpy_data , Algorithme , n_threads
	
#define DECLARE_INPUTS_PY_2(IntType)\
	py::array_t<IntType> numpy_hist, py::array_t<IntType> numpy_data , std::string Algorithme , int n_threads
	
#define INPUTS_PY_2 \
	numpy_hist , numpy_data , Algorithme , n_threads
///////////////////////////////////

template <class BinType>
class Histogram<uint8_t,BinType> : public Histogram_of_ints<uint8_t,BinType>
{
public : 
	Histogram( DECLARE_INPUTS(uint8_t) ): Histogram_of_ints<uint8_t,BinType>( INPUTS ) {}
	Histogram( DECLARE_INPUTS_PY_1(uint8_t) ): Histogram_of_ints<uint8_t,BinType>( INPUTS_PY_1 ) {}
	Histogram( DECLARE_INPUTS_PY_2(uint8_t) ): Histogram_of_ints<uint8_t,BinType>( INPUTS_PY_2 ) {}
	
	void execute();
};

template <class BinType>
class Histogram<int8_t,BinType> : public Histogram_of_ints<int8_t,BinType>
{
public : 
	Histogram( DECLARE_INPUTS(int8_t) ): Histogram_of_ints<int8_t,BinType>( INPUTS ) {}
	Histogram( DECLARE_INPUTS_PY_1(int8_t) ): Histogram_of_ints<int8_t,BinType>( INPUTS_PY_1 ) {}
	Histogram( DECLARE_INPUTS_PY_2(int8_t) ): Histogram_of_ints<int8_t,BinType>( INPUTS_PY_2 ) {}
	
	void execute(); 
};

// UINT16 AND INT16 //////////////////////////////////////////////
template <class BinType>
class Histogram<uint16_t,BinType> : public Histogram_of_ints<uint16_t,BinType>
{	
public : 
	Histogram( DECLARE_INPUTS(uint16_t) ): Histogram_of_ints<uint16_t,BinType>( INPUTS ) {}
	Histogram( DECLARE_INPUTS_PY_1(uint16_t) ): Histogram_of_ints<uint16_t,BinType>( INPUTS_PY_1 ) {}
	Histogram( DECLARE_INPUTS_PY_2(uint16_t) ): Histogram_of_ints<uint16_t,BinType>( INPUTS_PY_2 ) {}
	
	// Computes the Histogram for (8<b<=16)-bit samples in uint16 containers
	int sample_bit_length ;
	
	void execute();
};

template <class BinType>
class Histogram<int16_t,BinType> : public Histogram_of_ints<int16_t,BinType>
{
public : 
	Histogram( DECLARE_INPUTS(int16_t) ): Histogram_of_ints<int16_t,BinType>( INPUTS ) {}
	Histogram( DECLARE_INPUTS_PY_1(int16_t) ): Histogram_of_ints<int16_t,BinType>( INPUTS_PY_1 ) {}
	Histogram( DECLARE_INPUTS_PY_2(int16_t) ): Histogram_of_ints<int16_t,BinType>( INPUTS_PY_2 ) {}
	
	// Computes the Histogram for (8<b<=16)-bit samples in uint16 containers
	int sample_bit_length ;
	
	void execute();
};

///////////////////////////////////
// END of MACROS SCOPES
#undef DECLARE_INPUTS
#undef INPUTS 
#undef DECLARE_INPUTS_PY_1
#undef INPUTS_PY_1
#undef DECLARE_INPUTS_PY_2
#undef INPUTS_PY_2
///////////////////////////////////
