#pragma once

#include "../includes/common.h"
#include "../includes/omp_extra.h"
#include "../includes/buffered_array.h"

/*
Developer comment : 
	This class could be hard to maintain if you don't understan c++ multiple inheritance, templates and templates partial specialisation.
	see : 
		https://en.cppreference.com/book/intro/inheritance
		https://en.cppreference.com/book/intro/class_template
		https://en.cppreference.com/w/cpp/language/partial_specialization
*/

//// MOTHER OF ALL 2D HISTOGRAMS OF SIGNED INTEGERS
template <class IntType, class BinType>
class Histogram2D_of_ints
{	
	public : 
		uint64_t NofBins ;
		int n_threads;
		std::string Algorithme;
		Buffered_Array<BinType> np_histogram ; 
		
		// Contructor
		Histogram2D_of_ints( uint64_t NofBins , IntType* data_1 , IntType* data_2 , uint64_t L_data , std::string Algorithme , int n_threads ); 
		Histogram2D_of_ints( uint64_t NofBins, py::array_t<IntType> numpy_data_1 , py::array_t<IntType> numpy_data_2 , std::string Algorithme , int n_threads );
		Histogram2D_of_ints( py::array_t<IntType> numpy_hist, py::array_t<IntType> numpy_data_1 , py::array_t<IntType> numpy_data_2 , std::string Algorithme , int n_threads );
		// Destructor
		~Histogram2D_of_ints(){ p_historgam = NULL ; data_1 = NULL ; data_2 = NULL ; };
		
		// sets and gets
		int get_n_threads(){return n_threads;}
		void set_n_threads( int n_threads ) ;
		IntType* get_p_data_1(){return data_1;}
		void set_p_data_1( IntType* data_1 ){this->data_1 = data_1;} 
		IntType* get_p_data_2(){return data_2;}
		void set_p_data_2( IntType* data_2 ){this->data_2 = data_2;} 
		
	protected :
		BinType* p_historgam ; 
		IntType* data_1;
		IntType* data_2;
		uint64_t  L_data ;
		
		bool isPerfectSquare(uint64_t x){ double sr = sqrt(x); return ( ( (uint64_t)sr * (uint64_t)sr ) == x);} 
	
		void swap_histogram(BinType *hist, const int b);
		void swap_histogram2D(BinType *hist, const int b);
		static inline void basic( uint8_t* data_1 , uint8_t* data_2 , uint64_t L_data, BinType* hist );
		static inline void basic( uint16_t* data_1 , uint16_t* data_2 , uint64_t L_data, BinType* hist, int b );
		static inline void parallel( uint8_t* data_1 , uint8_t* data_2 , uint64_t L_data, BinType* hist );
		static inline void parallel( uint16_t* data_1 , uint16_t* data_2 , uint64_t L_data, BinType* hist, int b );
		static inline void atmc( uint16_t* data_1 , uint16_t* data_2 , uint64_t L_data, BinType* hist, int b );
		
};
