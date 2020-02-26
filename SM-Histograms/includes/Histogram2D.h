#pragma once

#include "../../SM-Omp_extra/includes/omp_extra.h"
#include "../../SM-Math_extra/math_extra.h"
#include "../../SM-Moments_cumulants/includes/moments_cumulants.h"

/*
# Uncomment to benchmark
*/
//#include "../../SM-Scoped_timer/includes/scoped_timer.h"

/*
https://en.cppreference.com/w/cpp/language/template_specialization
https://en.cppreference.com/w/cpp/language/member_template#Member_function_templates
*/


template < class BinType >
class Histogram2D
{
	public : 
		
		Histogram2D( uint64_t nofbins = 0 , int n_threads = 2  );
		~Histogram2D();
		
        // C++ INTERFACE
            // Core functions
		void accumulate(  double* data_1, double* data_2,  uint64_t L_data, double max ) ;
		void accumulate(  float* data_1, float* data_2,  uint64_t L_data, float max ) ;
		void accumulate(  uint8_t* data_1, uint8_t* data_2,  uint64_t L_data ) ;
		void accumulate(  uint16_t* data_1, uint16_t* data_2,  uint64_t L_data, int b ) ;
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
            
        double moment2D( uint exp_x = 0 , uint exp_y = 0  , double first_bin_center = 0 , double bin_width = 1.0);
        double moment2D_float( uint exp_x = 0 , uint exp_y = 0 , double max = 1.0 );
		
		double cumulant_ii( double first_bin_center = 0 , double bin_width = 1.0);
        double cumulant_ii_float( double max = 1.0 );
        
		double cumulant_jj( double first_bin_center = 0 , double bin_width = 1.0);
        double cumulant_jj_float( double max = 1.0 );
		
		double cumulant_iijj( double first_bin_center = 0 , double bin_width = 1.0);
        double cumulant_iijj_float( double max = 1.0 );
        
		
        //  Python interface
            // Core functions
		template<class FloatType>
		void accumulate_float_py(  py::array_t<FloatType> data_1, py::array_t<FloatType> data_2, FloatType max ) ;
	
		void accumulate_int_py(  py::array_t<uint8_t> data_1, py::array_t<uint8_t> data_2 );
		void accumulate_int_py(  py::array_t<uint16_t> data_1, py::array_t<uint16_t> data_2 , int b);
		
            // Sets and gets
		py::array_t<BinType> get_py();

        py::array_t<double> abscisse_float_py( double max );
        
        // Uncomment to benchmark
        uint64_t accumulate_timer ;
	
	protected :
		uint64_t nofbins ;
		int n_threads ;
		BinType* histogram ;
		uint8_t** hs ;
		
		// Checks
		void Check_parity() ;
		void Check_n_threads() ;

        // C++ INTERFACE
		void Allocate_all_heap();
		void Free_all_heap();
	
            // Core functions
		void to_hist_middleman_pragma( int this_thread , uint32_t bin );
		void reduction_and_reset();
		
		template<class FloatType>
		void what_bin_float( FloatType data_1 , FloatType data_2, FloatType max, FloatType bin_width, uint16_t* binx, uint16_t* biny) ;
};