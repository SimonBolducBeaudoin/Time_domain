#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <complex>


template <class Type>
class Buffered_Vector 
{
	public :
		// Contructor
		Buffered_Vector(uint64_t L = 0);
		// Destructor
		~Buffered_Vector();
		// Methods
		void recallocate(uint64_t L);	

		// Setters and getters
		Type* get_BufferData(){return BufferData ; } ;
		uint64_t get_BufferDimension(){return BufferDimension ; };
		// void set_BufferData(Type* BufferData){this->BufferData = BufferData; } 
		// void set_BufferDimension(uint64_t BufferDimension ){ this->BufferDimension = BufferDimension; }
	protected :
		uint64_t BufferDimension ; 
		Type* BufferData ; 
	
};

template <class Type>
class Buffered_Convolution 
{
	public :
		// Contructor
		Buffered_Convolution(uint64_t L_data = 1 , uint64_t L_kernel= 1);
		
		// Destructor
		~Buffered_Convolution();
		// Methods
		void recallocate(uint64_t l_data, uint64_t l_kernel);	

		// Setters and getters
		Type* get_full(){return data ; } ;
		Type* get_valid(){return data_valid ; } ;
		uint64_t get_full_size(){return l_full ; };
		uint64_t get_valid_size(){return l_valid ; };
	protected :
		uint64_t l_kernel ; 
		uint64_t l_data ; 
		uint64_t l_full ; 
		uint64_t l_valid ; 
		Type* data ;
		Type* data_valid ;
};
