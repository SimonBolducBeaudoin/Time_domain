#pragma once

#include "../includes/common.h"
#include "../includes/buffered_array.h"

#include "../includes/scoped_timer.h"

// primary template

template <class FloatType, class DataType>
class Mother_FFT
{
	// This class is meant to benchmark different implementation of the FFT using fftw
	public : 
		uint32_t L_FFT ;
		uint64_t L_data ;
		int nthreads ; 
		uint64_t Time_alloc;
		uint64_t Time_execution;
		uint64_t Time_preparation;
		
		Buffered_Array<DataType> np_data ; 
		Buffered_Array<std::complex<FloatType>> np_inplace_halfC; 
		Buffered_Array<FloatType> np_direct ; 
		
		// Contructor
		Mother_FFT(uint64_t L_data = 0, uint32_t L_FTT = 0 , int nthreads = 1 );
		// Destructor
		~Mother_FFT();
		
		// Methods :
		uint64_t get_L_data(){return L_data ; }  // Lenght(/number of elements) of the sedond input
		// Set and get for L_FFT
		uint64_t get_L_FFT(){return L_FFT;}
		void set_L_FFT( uint32_t L_FFT ) ;
		int get_nthreads(){return nthreads;}
		void set_nthreads( int nthreads ) ;
	
	protected :
		DataType*  p_data ; 
		std::complex<FloatType>* p_inplace_halfC;
		FloatType* p_direct;
			
		// Called in constructor
		void cast_and_alloc();
        
};

// primary template
// see : https://en.cppreference.com/w/cpp/language/partial_specialization
template <class FloatType, class DataType>
class FFT_Benchmarks 
{
};

// partial specialization #1
template <class DataType>
class FFT_Benchmarks<float,DataType> : public Mother_FFT<float,DataType>
{
	// This class is meant to benchmark different implementation of the FFT using fftw
	public : 
		int FFTWF_init_threads = fftwf_init_threads();
		fftwf_plan plan;
        
        // Contructor
		FFT_Benchmarks(uint64_t L_data = 1, uint32_t L_FTT = 1, int nthreads = 1 );
		// Destructor
		~FFT_Benchmarks();
        
        void prepare();
        void execute();
        
    private :
    
        // Called when executing convolution FFT
		void FFT_inplace();
		
};
// partial specialization #2
template <class DataType>
class FFT_Benchmarks<double,DataType> : public Mother_FFT<double,DataType>
{
	// This class is meant to benchmark different implementation of the FFT using fftw
	public : 
		int FFTW_init_threads = fftw_init_threads();
		fftw_plan plan;
        
        // Contructor
		FFT_Benchmarks(uint64_t L_data = 1, uint32_t L_FTT = 1 , int nthreads = 1 );
		// Destructor
		~FFT_Benchmarks();
        
        void prepare();
        void execute();
        
    private : 
        // Called when executing convolution FFT
		void FFT_inplace();
		
};
