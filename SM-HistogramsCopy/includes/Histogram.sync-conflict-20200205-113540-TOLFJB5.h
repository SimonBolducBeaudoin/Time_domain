#pragma once

#include "../../SM-Omp_extra/includes/omp_extra.h"

// https://en.cppreference.com/w/cpp/language/template_specialization
// https://en.cppreference.com/w/cpp/language/member_template#Member_function_templates

template < class BinType >
class Histogram
{
	public : 
		
		Histogram( uint64_t nofbins = 0 , int n_threads = 2  );
		~Histogram();
		
		// ACCUMULATION FUNCTIONS OVERLOADING 
		void accumulate(  double* data,  uint64_t L_data, double max ) ;
		void accumulate(  float* data,  uint64_t L_data, float max ) ;
		void accumulate(  uint8_t* data,  uint64_t L_data ) ;
		void accumulate(  uint16_t* data,  uint64_t L_data, int b ) ;
		// Did not implement methods for int8 and int16
		
		template<class DataType>
		void accumulate_float_py(  py::array_t<DataType> data, DataType max ) ;
		
		void accumulate_int_py(  py::array_t<uint8_t> data ) ;
		void accumulate_int_py(  py::array_t<uint16_t> data, int b ) ;
		
		BinType* get_pointer(){return histogram ;};
		uint64_t get_nofbins(){return nofbins ;};
		
		py::array_t<BinType> get_py();
		
	
	protected :
		uint64_t nofbins ;
		int n_threads ;
		BinType* histogram ;
		
		template<class FloatType>
		void float_to_hist(FloatType data, uint64_t L_data , FloatType max , FloatType bin_width );
		
};
