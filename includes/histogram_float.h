#pragma once

#include "../includes/common.h"
#include "../includes/buffered_array.h"

/*
Developer comment : 
	This class could be hard to maintain if you don't understan c++ multiple inheritance, templates and templates partial specialisation.
	see : 
		https://en.cppreference.com/book/intro/inheritance
		https://en.cppreference.com/book/intro/class_template
		https://en.cppreference.com/w/cpp/language/partial_specialization
*/

//////////////////////////////////////////////////////////////////////////////////////
//// MOTHER OF ALL HISTOGRAM_FLOATSS OF FLOATING POINTS
template <class FloatType, class BinType>
class Histogram_of_floats
{
	public : 
		uint64_t NofBins ;
		int n_threads;
		std::string Algorithme;
		Buffered_Array<BinType> np_histogram ; 
		
		FloatType max;
		
		Histogram_of_floats( uint64_t NofBins , FloatType* data , uint64_t L_data , FloatType max , std::string Algorithme , int n_threads ); 
		Histogram_of_floats( uint64_t NofBins, py::array_t<FloatType> numpy_data , FloatType max , std::string Algorithme , int n_threads );
		Histogram_of_floats( py::array_t<FloatType> numpy_hist, py::array_t<FloatType> numpy_data , FloatType max , std::string Algorithme , int n_threads );
		// Destructor
		~Histogram_of_floats(){ p_historgam = NULL ; data = NULL ; };
		
		// sets and gets
		int get_n_threads(){return n_threads;}
		void set_n_threads( int n_threads ) ;
		FloatType get_max(){return max;}
		void set_max(FloatType max){this->max = max; this->bin_width = 2*max/(this->NofBins);}
	
	protected :
		FloatType bin_width;
		BinType* p_historgam ; 
		FloatType* data;
		uint64_t  L_data ;
	
		inline void to_hist( const FloatType* data, uint64_t L_data, BinType* hist ) ;
		inline void basic( const FloatType* data, uint64_t L_data, BinType* hist ) ;
};