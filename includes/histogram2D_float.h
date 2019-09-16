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

//// MOTHER OF ALL 2D HISTOGRAMS OF FLOATING POINT
template <class FloatType, class BinType>
class Histogram2D_of_floats
{
	public :
		uint64_t NofBins ;
		uint8_t n_threads;
		std::string Algorithme;
		Buffered_Array<BinType> np_histogram ; 
	
		FloatType max;
		// Contructor
		Histogram2D_of_floats( uint64_t NofBins , FloatType* data_1 , FloatType* data_2 , uint64_t L_data , FloatType max , std::string Algorithme , int n_threads ); 
		Histogram2D_of_floats( uint64_t NofBins, py::array_t<FloatType> numpy_data_1 , py::array_t<FloatType> numpy_data_2 , FloatType max , std::string Algorithme , int n_threads );
		Histogram2D_of_floats( py::array_t<FloatType> numpy_hist, py::array_t<FloatType> numpy_data_1 , py::array_t<FloatType> numpy_data_2 , FloatType max , std::string Algorithme , int n_threads );
		// Destructor
		~Histogram2D_of_floats(){ p_historgam = NULL ; data_1 = NULL ; data_2 = NULL ; };
		
		// sets and gets
		int get_n_threads(){return n_threads;}
		void set_n_threads( int n_threads ) ;
		FloatType* get_data_1(){return data_1;}
		void set_data_1( FloatType* data_1 ){this->data_1 = data_1;} 
		FloatType* get_data_2(){return data_2;}
		void set_data_2( FloatType* data_2 ){this->data_2 = data_2;}
		FloatType get_max(){return max;}
		void set_max(FloatType max){this->max = max; this->bin_width = 2*max/(this->NofBins);}
		
	protected :
		FloatType bin_width;
		BinType* p_historgam ; 
		FloatType* data_1;
		FloatType* data_2;
		uint64_t  L_data ;
		
		bool isPerfectSquare(uint64_t x){ double sr = sqrt(x); return ( ( (uint64_t)sr * (uint64_t)sr ) == x);} 
	
		static inline void to_hist_middleman( uint8_t* h, BinType* hist, int bin ) ;
		static inline void to_hist_middleman_pragma( uint8_t* h, BinType* hist, int bin );
		inline void what_bin( const FloatType* data_1 , const FloatType* data_2, uint16_t* binx, uint16_t* biny) ;
		inline void basic( const FloatType* data_1 , const FloatType* data_2, uint64_t L_data, int NofBins , BinType* hist ) ;
		inline void basic_middleman( const FloatType* data_1 , const FloatType* data_2, uint64_t L_data, int NofBins , BinType* hist ) ;
};