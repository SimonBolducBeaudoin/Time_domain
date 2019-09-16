#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <complex>


template <class Type>
class Buffered_Array 
{
	public :
		// Contructor
		Buffered_Array(uint64_t L = 0);
		// Destructor
		~Buffered_Array();
		// Methods
		void recallocate(uint64_t L);	

		// Setters and getters
		Type* get_BufferData(){return BufferData ; } ;
		uint64_t get_BufferDimension(){return BufferDimension ; };
		void set_BufferData(Type* BufferData){this->BufferData = BufferData; } 
		void set_BufferDimension(uint64_t BufferDimension ){ this->BufferDimension = BufferDimension; }
	private :
		uint64_t BufferDimension ; 
		Type* BufferData ; 
	
};
