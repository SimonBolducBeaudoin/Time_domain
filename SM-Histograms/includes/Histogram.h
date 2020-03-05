#pragma once

#include "../../SM-Multi_array/Multi_array.h"
#include "../../SM-Moments_cumulants/includes/moments_cumulants.h"
#include "../../SM-Omp_extra/includes/omp_extra.h"

/*
	TODOS
	- Quality of life : add a function that recovers 1D histograms from 2D histogram ?
	- integrate Multi_array into histogram classes for better memory locality
	- Modify Multi_array for custom memory organisation (optimal for centered biased histograms)
*/


/*
# Uncomment to benchmark
*/
//#include "../../SM-Scoped_timer/includes/scoped_timer.h"

/*
https://en.cppreference.com/w/cpp/language/template_specialization
https://en.cppreference.com/w/cpp/language/member_template#Member_function_templates
*/

template < class BinType >
class Histogram
{
	public : 
		
		Histogram( uint64_t nofbins = 0 , int n_threads = 2  );
		~Histogram();
		
		// C++ INTERFACE
            // Core functions
		void accumulate(  double* data,  uint64_t L_data, double max ) ;
		void accumulate(  float* data,  uint64_t L_data, float max ) ;
		void accumulate(  uint8_t* data,  uint64_t L_data ) ;
		void accumulate(  uint16_t* data,  uint64_t L_data, int b ) ;
		// Did not implement methods for int8 and int16
        
            // Sets and gets
        BinType* get_pointer(){return histogram ;};
		uint64_t get_nofbins(){return nofbins ;};
        
            // Histogram properties
        /*
        moment_k calls directly the function moment_k from SM-Moment_cumulants
            and uses Histogram's meta-infos.
        moment_k_float calls the same function but is used when working with a histogram of floating point
            centered around zero and upperbounded by max.
        */
        double moment_k( uint k = 1, double first_bin_center = 0 , double bin_width = 1.0);
        double moment_k_float( uint k = 1, double max = 1.0 );
        
        double centered_moment_k( uint k = 2, double first_bin_center = 0 , double bin_width = 1.0);
        double centered_moment_k_float( uint k = 2, double max = 1.0 );
        
        double cumulant_k( uint k = 1, double first_bin_center = 0 , double bin_width = 1.0);
        double cumulant_k_float( uint k = 1, double max = 1.0);
		
        // Python interface
            
            // Core functions
        template<class DataType>
		void accumulate_float_py(  py::array_t<DataType> data, DataType max ) ;
        
		void accumulate_int_py(  py::array_t<uint8_t> data ) ;
		void accumulate_int_py(  py::array_t<uint16_t> data, int b ) ;
        
            // Sets and gets
		py::array_t<BinType> get_py();
        py::array_t<double> abscisse_float_py( double max );
		
            // Uncomment to benchmark
        // uint64_t accumulate_timer ;
        
	protected :
		uint64_t nofbins ;
		int n_threads ;
		BinType* histogram ;
        
        Multi_array<double,1> data ;
		
		// Checks
		void Check_parity() ;
		void Check_n_threads() ;
        
        // C++ INTERFACE
            // Core functions
		template<class FloatType>
		void float_to_hist(FloatType data, BinType* histogram , FloatType max , FloatType bin_width );
        
     	
};
