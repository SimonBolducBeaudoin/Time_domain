#include "../includes/buffered_array_py.h"

#define PY_BUFFERED_ARRAY(DataType)\
		py::class_<Buffered_Array<DataType>>( m , "Buffered_Array_"#DataType, py::buffer_protocol())\
		.def_buffer([](Buffered_Array<DataType> &instance) -> py::buffer_info {\
			return py::buffer_info(\
				instance.get_BufferData(), sizeof(DataType),\
				py::format_descriptor<DataType>::format(), 1 , {instance.get_BufferDimension()}, {sizeof(DataType)}\
			);\
		})\
		;
		
void init_buffered_array(py::module &m)
{
	PY_BUFFERED_ARRAY(int16_t);
	PY_BUFFERED_ARRAY(float);
	PY_BUFFERED_ARRAY(double);
	PY_BUFFERED_ARRAY(complex_f); 
	PY_BUFFERED_ARRAY(complex_d);
}

// CLOSE MACRO SCOPES
#undef PY_BUFFERED_ARRAY