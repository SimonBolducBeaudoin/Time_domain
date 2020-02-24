#pragma once

#include "../../SM-Omp_extra/includes/omp_extra.h"

// https://en.cppreference.com/w/cpp/language/template_specialization
// https://en.cppreference.com/w/cpp/language/member_template#Member_function_templates

template < class BinType >
class Histogram2D
{
	public : 
		
		Histogram2D( uint64_t nofbins = 0 , int n_threads = 2  );
		~Histogram2D();
		
		// ACCUMULATION FUNCTIONS OVERLOADING 
		void accumulate(  double* data_1, double* data_2,  uint64_t L_data, double max ) ;
		void accumulate(  float* data_1, float* data_2,  uint64_t L_data, float max ) ;
		void accumulate(  uint8_t* data_1, uint8_t* data_2,  uint64_t L_data ) ;
		void accumulate(  uint16_t* data_1, uint16_t* data_2,  uint64_t L_data, int b ) ;
		// Did not implement methods for int8 and int16
		
		template<class FloatType>
		void accumulate_float_py(  py::array_t<FloatType> data_1, py::array_t<FloatType> data_2, FloatType max ) ;
	
		void accumulate_int_py(  py::array_t<uint8_t> data_1, py::array_t<uint8_t> data_2 );
		void accumulate_int_py(  py::array_t<uint16_t> data_1, py::array_t<uint16_t> data_2 , int b);
		
		BinType* get_pointer(){return histogram ;};
		uint64_t get_nofbins(){return nofbins ;};
		
		py::array_t<BinType> get_py();
		
	
	protected :
		uint64_t nofbins ;
		int n_threads ;
		BinType* histogram ;
		uint8_t** hs ;
		
		// template<class FloatType>
		// void float_to_hist(FloatType data, uint64_t L_data , FloatType max , FloatType bin_width );
		
		void Allocate_all_heap();
		void Free_all_heap();
	
		// void to_hist_middleman( uint8_t* h, BinType* hist, int bin ) ;
		void to_hist_middleman_pragma( int this_thread , uint32_t bin );
		void reduction_and_reset();
		
		template<class FloatType>
		void what_bin_float( FloatType data_1 , FloatType data_2, FloatType max, FloatType bin_width, uint16_t* binx, uint16_t* biny) ;
		// void basic( const FloatType* data_1 , const FloatType* data_2, uint64_t L_data, int nofbins , BinType* hist ) ;
		// void basic_middleman( const FloatType* data_1 , const FloatType* data_2, uint64_t L_data, int nofbins , BinType* hist ) ;
		
};