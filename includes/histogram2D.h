#pragma once

#include "../includes/Histogram2D_float.h"
#include "../includes/Histogram2D_int.h"

//////////////////////////////////////////////////////////////////////////////////////
// Histogram2D primary template
template <class IntType, class BinType>
class Histogram2D
{};

///////////////////////////////////
// BEGINING OF MACROS SCOPE
#define DECLARE_INPUTS(FloatType)\
	uint64_t NofBins, FloatType* data_1 , FloatType* data_2 , uint64_t L_data , FloatType max , std::string Algorithme , int n_threads
	
#define INPUTS \
	NofBins , data_1 , data_2 , L_data , max , Algorithme , n_threads

#define DECLARE_INPUTS_PY_1(FloatType)\
	uint64_t NofBins, py::array_t<FloatType> numpy_data_1 , py::array_t<FloatType> numpy_data_2 , FloatType max , std::string Algorithme , int n_threads
	
#define INPUTS_PY_1 \
	NofBins , numpy_data_1 , numpy_data_2 , max , Algorithme , n_threads
	
#define DECLARE_INPUTS_PY_2(FloatType)\
	py::array_t<FloatType> numpy_hist, py::array_t<FloatType> numpy_data_1 , py::array_t<FloatType> numpy_data_2 , FloatType max , std::string Algorithme , int n_threads
	
#define INPUTS_PY_2 \
	numpy_hist , numpy_data_1 , numpy_data_2 , max , Algorithme , n_threads
///////////////////////////////////

// partial specializations
template <class BinType>
class Histogram2D<float,BinType> : public Histogram2D_of_floats<float,BinType>
{
public : 
	Histogram2D( DECLARE_INPUTS(float) ): Histogram2D_of_floats<float,BinType>( INPUTS ) {}
	Histogram2D( DECLARE_INPUTS_PY_1(float) ): Histogram2D_of_floats<float,BinType>( INPUTS_PY_1 ) {}
	Histogram2D( DECLARE_INPUTS_PY_2(float) ): Histogram2D_of_floats<float,BinType>( INPUTS_PY_2 ) {}

	void execute() ;
};

template <class BinType>
class Histogram2D<double,BinType> : public Histogram2D_of_floats<double,BinType>
{
public : 
	Histogram2D( DECLARE_INPUTS(double) ): Histogram2D_of_floats<double,BinType>( INPUTS ) {}
	Histogram2D( DECLARE_INPUTS_PY_1(double) ): Histogram2D_of_floats<double,BinType>( INPUTS_PY_1 ) {}
	Histogram2D( DECLARE_INPUTS_PY_2(double) ): Histogram2D_of_floats<double,BinType>( INPUTS_PY_2 ) {}

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

///////////////////////////////////
// BEGINING OF MACROS SCOPE
#define DECLARE_INPUTS(IntType)\
	uint64_t NofBins , IntType* data_1 , IntType* data_2 , uint64_t L_data , std::string Algorithme , int n_threads
	
#define INPUTS \
	NofBins , data_1 , data_2 ,L_data , Algorithme , n_threads

#define DECLARE_INPUTS_PY_1(IntType)\
	uint64_t NofBins, py::array_t<IntType> numpy_data_1 , py::array_t<IntType> numpy_data_2 , std::string Algorithme , int n_threads
	
#define INPUTS_PY_1 \
	NofBins , numpy_data_1 , numpy_data_2 , Algorithme , n_threads
	
#define DECLARE_INPUTS_PY_2(IntType)\
	py::array_t<IntType> numpy_hist, py::array_t<IntType> numpy_data_1 , py::array_t<IntType> numpy_data_2 , std::string Algorithme , int n_threads
	
#define INPUTS_PY_2 \
	numpy_hist , numpy_data_1 , numpy_data_2 , Algorithme , n_threads
///////////////////////////////////

// partial specializations
// UINT8 AND INT8 //////////////////////////////////////////////
template <class BinType>
class Histogram2D<uint8_t,BinType> : public Histogram2D_of_ints<uint8_t,BinType>
{
public : 
	Histogram2D( DECLARE_INPUTS(uint8_t) ): Histogram2D_of_ints<uint8_t,BinType>( INPUTS ) {}
	Histogram2D( DECLARE_INPUTS_PY_1(uint8_t) ): Histogram2D_of_ints<uint8_t,BinType>( INPUTS_PY_1 ) {}
	Histogram2D( DECLARE_INPUTS_PY_2(uint8_t) ): Histogram2D_of_ints<uint8_t,BinType>( INPUTS_PY_2 ) {}

	void  execute();
	
};

template <class BinType>
class Histogram2D<int8_t,BinType> : public Histogram2D_of_ints<int8_t,BinType>
{
public : 
	Histogram2D( DECLARE_INPUTS(int8_t) ): Histogram2D_of_ints<int8_t,BinType>( INPUTS ) {}
	Histogram2D( DECLARE_INPUTS_PY_1(int8_t) ): Histogram2D_of_ints<int8_t,BinType>( INPUTS_PY_1 ) {}
	Histogram2D( DECLARE_INPUTS_PY_2(int8_t) ): Histogram2D_of_ints<int8_t,BinType>( INPUTS_PY_2 ) {}

	void  execute();
	
};

// UINT16 AND INT16 //////////////////////////////////////////////
template <class BinType>
class Histogram2D<uint16_t,BinType> : public Histogram2D_of_ints<uint16_t,BinType>
{
public : 
	Histogram2D( DECLARE_INPUTS(uint16_t) ): Histogram2D_of_ints<uint16_t,BinType>( INPUTS ) {}
	Histogram2D( DECLARE_INPUTS_PY_1(uint16_t) ): Histogram2D_of_ints<uint16_t,BinType>( INPUTS_PY_1 ) {}
	Histogram2D( DECLARE_INPUTS_PY_2(uint16_t) ): Histogram2D_of_ints<uint16_t,BinType>( INPUTS_PY_2 ) {}
	// Computes the histogram for (8<b<=16)-bit samples in uint16 containers
	int sample_bit_length ;
	
	void  execute();
	
};

template <class BinType>
class Histogram2D<int16_t,BinType> : public Histogram2D_of_ints<int16_t,BinType>
{
public : 
	Histogram2D( DECLARE_INPUTS(int16_t) ): Histogram2D_of_ints<int16_t,BinType>( INPUTS ) {}
	Histogram2D( DECLARE_INPUTS_PY_1(int16_t) ): Histogram2D_of_ints<int16_t,BinType>( INPUTS_PY_1 ) {}
	Histogram2D( DECLARE_INPUTS_PY_2(int16_t) ): Histogram2D_of_ints<int16_t,BinType>( INPUTS_PY_2 ) {}
	// Computes the histogram for (8<b<=16)-bit samples in uint16 containers
	int sample_bit_length ;
	
	void execute() ;
};

