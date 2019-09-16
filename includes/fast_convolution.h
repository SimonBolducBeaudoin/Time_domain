#pragma once

#include "../includes/common.h"
#include "../includes/buffered_array.h"

template <class OutputType, class KernelType, class DataType, class ComplexType>
class Mother_Fast_Conv
{
// Nomenclature
// h(f) = \int f(t) g(t-tau) dt
// g is the one that is flipped on the x axis.

// Parameters at contructions/instanciation
	// I used mane private/public sections to ensure that attributes are declared in the same order as in the constructor function. (ifnot we get a warning)
	public : 
		uint32_t L_FFT;// Lenght(/number of elements) of the FFT used for FFT convolution
		std::string Convolution_type; // direct or FFT
		
		Buffered_Array<DataType> np_data ; 
		Buffered_Array<KernelType> np_kernel; 
		Buffered_Array<OutputType> np_output ;  
		Buffered_Array<OutputType> np_output_valid ;
		
		// Contructor
		Mother_Fast_Conv(uint64_t L_kernel = 2,uint64_t L_data = 16, uint32_t L_FFT = 8, std::string Convolution_type = "fft");
		// Destructor
		~Mother_Fast_Conv();
		
		// Methods :
		// get methods (for readonly variables)
		uint64_t get_L_kernel(){return L_kernel ; } 
		uint64_t get_L_data(){return L_data ; }  // Lenght(/number of elements) of the sedond input
		// Set and get for L_FFT
		uint64_t get_L_FFT(){return L_FFT;}
		void set_L_FFT( uint32_t L_FFT );
		
		void print_all_attributes();
			
	protected :
		uint64_t L_kernel; // Lenght(/number of elements) of the first input
		uint64_t L_data; // Lenght(/number of elements) of the sedond input
		// Deduced from the inputs of the constructor
		uint64_t L_full ;
		uint64_t L_valid ;
		
		// Important pointers
		KernelType* kernel;
		DataType*  data ; 
		OutputType* output ;
			
		// For FFT convol
		uint64_t L_chunk ; // The length of a chunk
		uint64_t N_chunks ; // The number of chunks
		uint64_t L_reste ; // The length of what didn't fit into chunks
		
		// Called in constructor
		void checks();
		void cast_and_alloc();
		
		//Called when executing direct convolution
		void Conv_basic( KernelType* f , DataType* g , OutputType* h)const ;		
}; 

template <class OutputType, class KernelType, class DataType, class ComplexType>
class Fast_Conv{};

template <class OutputType, class KernelType, class DataType>
class Fast_Conv<OutputType,KernelType,DataType,complex_f>: public Mother_Fast_Conv<OutputType,KernelType,DataType,complex_f>
{
	public :
		Fast_Conv(uint64_t L_kernel , uint64_t L_data, uint32_t L_FFT , std::string Convolution_type)
			: Mother_Fast_Conv<OutputType,KernelType,DataType,complex_f>(L_kernel , L_data , L_FFT , Convolution_type)
			{};
		void execute();
	private:
	// Called when executing convolution FFT
		inline void Conv_FFT(const KernelType* Kernel ,const DataType* Data , OutputType* Out );
		inline void FFT_kernel(const KernelType* In, complex_f* Out );
}; 

template <class OutputType, class KernelType, class DataType>
class Fast_Conv<OutputType,KernelType,DataType,complex_d>: public Mother_Fast_Conv<OutputType,KernelType,DataType,complex_d>
{
	public :
		Fast_Conv(uint64_t L_kernel , uint64_t L_data, uint32_t L_FFT , std::string Convolution_type)
			: Mother_Fast_Conv<OutputType,KernelType,DataType,complex_d>(L_kernel , L_data , L_FFT , Convolution_type)
			{};
		void execute();
	private:
	// Called when executing convolution FFT
		inline void Conv_FFT(const KernelType* Kernel ,const DataType* Data , OutputType* Out );
		inline void FFT_kernel(const KernelType* In, complex_d* Out );	
}; 




