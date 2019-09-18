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

//// MOTHER OF ALL HISTOGRAM_INTSS 1D OF INTEGERS
template <class IntType, class BinType>
class Histogram_of_ints 
{
	public :
		uint64_t nofbins ;
		int n_threads;
		std::string algorithme;
		Buffered_Array<BinType> np_histogram ;
		
		Histogram_of_ints( uint64_t nofbins , IntType* data , uint64_t L_data, std::string algorithme , int n_threads ); 
		Histogram_of_ints( uint64_t nofbins, py::array_t<IntType> numpy_data , std::string algorithme , int n_threads );
		Histogram_of_ints( py::array_t<IntType> numpy_hist, py::array_t<IntType> numpy_data , std::string algorithme , int n_threads );
		// Destructor
		~Histogram_of_ints(){ p_historgam = NULL ; data = NULL ; };
		
		// sets and gets
		int get_n_threads(){return n_threads;}
		void set_n_threads( int n_threads ) ;
		IntType* get_data(){return data;}
		void set_data( IntType* data ){this->data = data;} 
		
	protected :
		BinType* p_historgam ; 
		IntType* data;
		uint64_t  L_data ;
		
		static void swap_histogram(BinType *hist, const int b);
		static inline void basic( const uint8_t* data , uint64_t L_data , BinType* hist ) ;
		static inline void basic( const uint16_t* data , uint64_t L_data , BinType* hist , int b) ;
		static inline void parallel( const uint8_t* data, uint64_t L_data, BinType* hist );
		static inline void parallel( const uint16_t* data, uint64_t L_data, BinType* hist , int b);
};


