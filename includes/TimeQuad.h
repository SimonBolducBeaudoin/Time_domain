#pragma once

#include "../includes/common.h"
#include "../includes/buffered_array.h"

template <class FloatType, class DataType>
class TimeQuad
{
// Nomenclature
// h(f) = \int f(t) g(t-tau) dt
// g is the one that is flipped on the x axis.

// Parameters at contructions/instanciation
	// I used mane private/public sections to ensure that attributes are declared in the same order as in the constructor function. (ifnot we get a warning)
	private :
		uint64_t L_kernel; // Lenght(/number of elements) of the first input
		uint64_t L_data; // Lenght(/number of elements) of the sedond input
	public : 
		uint32_t L_FFT;// Lenght(/number of elements) of the FFT used for FFT convolution
	private :
		FloatType dt; // [ns]
	public :
		std::string Convolution_type; // direct or FFT
		
		Buffered_Array<DataType> np_data ; 
		Buffered_Array<FloatType> np_kernel_p ; 
		Buffered_Array<FloatType> np_kernel_q ; 
		Buffered_Array<FloatType> np_p ; 
		Buffered_Array<FloatType> np_q ; 
		Buffered_Array<FloatType> np_p_valid ;
		Buffered_Array<FloatType> np_q_valid ;
        
		// Contructor
		TimeQuad(uint64_t L_kernel = 2,uint64_t L_data = 16, 
					uint32_t L_FFT = 8, FloatType dt= 0.03125,
					std::string Convolution_type = "fft");
		// Destructor
		~TimeQuad();
		
		// Methods :
		// get methods (for readonly variables)
		uint64_t get_L_kernel(){return L_kernel ; } 
		uint64_t get_L_data(){return L_data ; }  // Lenght(/number of elements) of the sedond input
		FloatType get_dt(){return dt ; } // [ns]
		// Set and get for L_FFT
		uint64_t get_L_FFT(){return L_FFT;}
		void set_L_FFT( uint32_t L_FFT );
		
		void print_all_attributes();
		
		void execute();
		
	private :
		// Deduced from the inputs of the constructor
		uint64_t L_full ;
		uint64_t L_valid ;
		
		// Important pointers
		FloatType* p_kernel_p ;
		FloatType* p_kernel_q ;
		DataType*  p_data ; 
		FloatType* p_p ;
		FloatType* p_q ;
			
		// For FFT convol
		uint64_t L_chunk ; // The length of a chunk
		uint64_t N_chunks ; // The number of chunks
		uint64_t L_reste ; // The length of what didn't fit into chunks
		
		// Called in constructor
		void checks();
		void cast_and_alloc();
		void make_kernels( )const ;
		
		// Called when executing convolution FFT
		void Conv_FFT(const FloatType* F ,const DataType* G , FloatType* H );
		void Conv_FFT_kernel(const FloatType* Input , std::complex<FloatType>* Output );
		
		//Called when executing direct convolution
		void Conv_Directe(  FloatType* f ,  DataType* g , FloatType* h)const;
		
}; 
