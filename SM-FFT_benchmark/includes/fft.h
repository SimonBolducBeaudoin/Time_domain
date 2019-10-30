#pragma once

#include "../../includes/header_common.h"
#include "../../SM-Buffered_arrays/includes/buffered_arrays.h"
#include "../../SM-Omp_extra/includes/omp_extra.h"

#include "../../SM-Scoped_timer/includes/scoped_timer.h"

// primary template

template <class FloatType, class DataType>
class Mother_FFT
{
	// This class is meant to benchmark different implementation of the FFT using fftw
	public : 
		uint32_t L_FFT ;
		uint64_t L_data ;
		int n_threads ;
		
		Buffered_Vector<DataType> np_data ; 
		// py::array_t<std::complex<FloatType> , py::array::c_style >  np_inplace_halfC; 
		// py::array_t<FloatType , py::array::c_style> np_direct ; 
		// Buffered_Vector<DataType> np_data ; 
		Buffered_Vector<std::complex<FloatType>> np_inplace_halfC; 
		Buffered_Vector<FloatType> np_direct ; 
		
		// Contructor
		Mother_FFT(uint64_t L_data = 0, uint32_t L_FTT = 0 , int n_threads = 1 );
		// Destructor
		~Mother_FFT();
		
		// Methods :
		uint64_t get_L_data(){return L_data ; }  // Lenght(/number of elements) of the sedond input
		// Set and get for L_FFT
		uint64_t get_L_FFT(){return L_FFT;}
		int get_n_threads(){return n_threads;}
		
	protected:
		DataType*  p_data ; 
		std::complex<FloatType>* p_inplace_halfC;
		FloatType* p_direct;
		
};

// primary template
// see : https://en.cppreference.com/w/cpp/language/partial_specialization
template <class FloatType, class DataType>
class FFT 
{
};

// partial specialization #1
template <class DataType>
class FFT<float,DataType> : public Mother_FFT<float,DataType>
{
	// This class is meant to benchmark different implementation of the FFT using fftw
	public : 
        // Contructor
		FFT(uint64_t L_data = 1, uint32_t L_FTT = 1, int n_threads = 1 );
		// Destructor
		~FFT();
        
		void set_L_FFT( uint32_t L_FFT ) ;
		void set_n_threads( int n_threads ) ;
        void execute();
		
    private :
		// int FFTWF_init_threads = fftwf_init_threads();
		fftwf_plan plan;
    
		void prepare();
		void FFT_inplace();
		void free_heap();
		
};
// partial specialization #2
template <class DataType>
class FFT<double,DataType> : public Mother_FFT<double,DataType>
{
	// This class is meant to benchmark different implementation of the FFT using fftw
	public : 
		
        // Contructor
		FFT(uint64_t L_data = 1, uint32_t L_FTT = 1 , int n_threads = 1 );
		// Destructor
		~FFT();
        
        void set_L_FFT( uint32_t L_FFT ) ;
		void set_n_threads( int n_threads ) ;
        void execute();
        
    private : 
		// int FFTW_init_threads = fftw_init_threads();
		fftw_plan plan;
		
		void prepare();
		void FFT_inplace();
		void free_heap();		
};
