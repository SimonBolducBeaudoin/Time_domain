#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <complex>

#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

template <class Type>
class Numpy_Vector 
{
	public :
		// Contructor
		Numpy_Vector(uint64_t L = 0);
		// Destructor
		~Numpy_Vector();
		// Methods
		void recallocate(uint64_t L);	

		// Setters and getters
		Type* get_BufferData(){ return BufferData ; } ;
		uint64_t get_BufferDimension(){ return BufferDimension ; };
		
		py::buffer_info info ;
		py::array_t<Type , py::array::c_style> numpy_array ; //by reference
	private :
		uint64_t BufferDimension ; 
		Type* BufferData ; 
};
