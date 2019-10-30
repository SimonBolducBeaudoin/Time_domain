#include "../includes/buffered_arrays_py.h"

#define PY_BUFFERED_VECTOR(DataType)\
	py::class_<Buffered_Vector<DataType>>( m , "Buffered_Vector_"#DataType, py::buffer_protocol())\
		.def_buffer([](Buffered_Vector<DataType> &instance) -> py::buffer_info\
        {\
			return py::buffer_info(\
				instance.get_BufferData(), sizeof(DataType),\
				py::format_descriptor<DataType>::format(), 1 , {instance.get_BufferDimension()}, {sizeof(DataType)}\
			);\
		})\
		;
		
// The following function can be called inside a pybind11 module to intenciate all buffered_arrays python class.
void init_buffered_arrays(py::module &m)
{
	PY_BUFFERED_VECTOR(int16_t);
	PY_BUFFERED_VECTOR(float);
	PY_BUFFERED_VECTOR(double);
	PY_BUFFERED_VECTOR(complex_f); 
	PY_BUFFERED_VECTOR(complex_d);
}

// CLOSE MACRO SCOPES
#undef PY_BUFFERED_VECTOR