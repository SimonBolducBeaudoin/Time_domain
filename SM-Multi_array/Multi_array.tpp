#include<malloc.h>

#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace pybind11::literals;
typedef std::complex<float> complex_f;
typedef std::complex<double> complex_d;
typedef unsigned int uint;

// see : https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/aligned-malloc?view=vs-2019
// https://stackoverflow.com/questions/44659924/returning-numpy-arrays-via-pybind11
void* _128bit_aligned_malloc( size_t size )
{
	void    *ptr;
    size_t  alignment = 16 ;
	
	ptr = _aligned_malloc(size, alignment);
    if (ptr == NULL) {throw std::runtime_error("Error allocation aligned memory.");}
	
    /* 
	if (((unsigned long long)ptr % alignment ) == 0)
        printf_s( "This pointer, %p, is aligned on %zu\n",
                  ptr, alignment);
    else
        printf_s( "This pointer, %p, is not aligned on %zu\n",
                  ptr, alignment); 
	*/
	// printf("_128bit_aligned_malloc");
	return ptr ;
};

template<class Type>
Multi_array<Type,1>::Multi_array
( 
	uint n_i , // Number of elements in i
	void* (*alloc_func)(size_t size) , // Custom allocation function 
	void (*free_func)(void* ptr)
)
: 
	alloc_func(alloc_func) , 
	free_func(free_func) , 
	ptr( (Type*)alloc_func(n_i*sizeof(Type)) ),
	n_i(n_i),
	stride_i(sizeof(Type))
{};

template<class Type>
Multi_array<Type,1>::Multi_array( Type* ptr , uint n_i )
:
	alloc_func(NULL), /* No memory manegement allowed */
	free_func(NULL),
	ptr(ptr),
	n_i(n_i),
	stride_i(sizeof(Type))
{};

template<class Type>
Multi_array<Type,1> Multi_array<Type,1>::numpy( py::array_t<Type, py::array::c_style> np_array )
{
	
	py::buffer_info buffer = np_array.request() ;
	
	if (buffer.ndim != 1) 
    {
		throw std::runtime_error("Number of dimensions must be one");
	}
	
	return Multi_array<Type,1>( (Type*)buffer.ptr , buffer.shape[0] );
};



template<class Type>
Multi_array<Type,1>::~Multi_array()
{
	_free_func();
};

template<class Type>
void Multi_array<Type,1>::_free_func()
{
	
	if ( (free_func!=NULL) and (ptr!=NULL) )
	{  free_func(ptr) ;  }
	ptr = NULL ;	
};

// OPERATOR OVERLOAD
template<class Type>
inline Type& Multi_array<Type,1>::operator()( uint i )
{
	return ptr[i] ;
};

template<class Type>
inline Type& Multi_array<Type,1>::operator()( uint i ) const
{
	return ptr[i] ;
};
////

template<class Type>
inline Type* Multi_array<Type,1>::get()
{
	return ptr ;
};



template<class Type>
py::array_t<Type, py::array::c_style> Multi_array<Type,1>::get_py_copy()
{
	// This class keeps ownership of the allocated memory. 
	// It will be freed only when the object is garbage collected in python.
	return py::array_t<Type, py::array::c_style>
	(
		{n_i},      // shape
		{stride_i},   // C-style contiguous strides for double
		ptr      	// the data pointer
	);
};

template<class Type>
py::array_t<Type, py::array::c_style> Multi_array<Type,1>::get_py_no_copy()
{
	// This class keeps ownership of the allocated memory. 
	// It will be freed only when the object is garbage collected in python.
	py::capsule free_dummy(	ptr, [](void *f){;} );
	return py::array_t<Type, py::array::c_style>
	(
		{n_i},      // shape
		{stride_i},   // C-style contiguous strides for double
		ptr ,       // the data pointer
		free_dummy // numpy array references this parent
	);
};

template<class Type>
Multi_array<Type,2>::Multi_array
( 
	uint n_j , // Number of elements in j
	uint n_i , // Number of elements in i
	void* (*alloc_func)(size_t size) , // Custom allocation function 
	void (*free_func)(void* ptr)
)
: 
	alloc_func(alloc_func) , 
	free_func(free_func) , 
	ptr( (Type*)alloc_func( n_j*n_i*sizeof(Type)) ),
	n_j(n_j) , n_i(n_i),
	stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
	steps_j(n_i)
{};
	
template<class Type>
Multi_array<Type,2>::Multi_array( Type* ptr , uint n_j , uint n_i )
:
	alloc_func(NULL), /* No memory manegement allowed */
	free_func(NULL),
	ptr(ptr),
	n_j(n_j), n_i(n_i),
	stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
	steps_j(n_i)
{};

template<class Type>
Multi_array<Type,2> Multi_array<Type,2>::numpy( py::array_t<Type, py::array::c_style> np_array )
{
	py::buffer_info buffer = np_array.request() ;
	
	if (buffer.ndim != 2) 
    {
		throw std::runtime_error("Number of dimensions must be two");
	}
	return Multi_array<Type,2>( (Type*)buffer.ptr , buffer.shape[0] , buffer.shape[1] );
};

template<class Type>
Multi_array<Type,2>::~Multi_array()
{
	_free_func();
};

template<class Type>
void Multi_array<Type,2>::_free_func()
{	
	
	if ( (free_func!=NULL) and (ptr!=NULL) )
	{free_func(ptr) ; }
	ptr = NULL ;
};

// OPERATOR OVERLOAD 
template<class Type>
inline Type& Multi_array<Type,2>::operator ()( uint j , uint i )
{
	return ptr[steps_j*j+i] ;
};

template<class Type>
inline Type* Multi_array<Type,2>::operator()( uint j )
{
	return ptr + steps_j*j ;
};

template<class Type>
inline Type* Multi_array<Type,2>::operator[]( uint j )
{
	return ptr + steps_j*j ;
};

template<class Type>
inline Type& Multi_array<Type,2>::operator ()( uint j , uint i ) const
{
	return ptr[steps_j*j+i] ;
};

template<class Type>
inline Type* Multi_array<Type,2>::operator()( uint j ) const
{
	return ptr + steps_j*j ;
};

template<class Type>
inline Type* Multi_array<Type,2>::operator[]( uint j ) const
{
	return ptr + steps_j*j ;
};
////

template<class Type>
Type* Multi_array<Type,2>::get()
{
	return ptr ;
};
	
template<class Type>
py::array_t<Type, py::array::c_style> Multi_array<Type,2>::get_py_copy()
{
	// Numpy will copy the array when using the assignement operator=
	return py::array_t<Type, py::array::c_style>
	(
		{n_j,n_i},      // shape
		{stride_j,stride_i},   // C-style contiguous strides for double
		ptr       // the data pointer
	);
};

template<class Type>
py::array_t<Type, py::array::c_style> Multi_array<Type,2>::get_py_no_copy()
{
	// Numpy will not copy the array when using the assignement operator=
	py::capsule free_dummy(	ptr, [](void *f){;} );
	
	return py::array_t<Type, py::array::c_style>
	(
		{n_j,n_i},      // shape
		{stride_j,stride_i},   // C-style contiguous strides for double
		ptr ,       // the data pointer
		free_dummy // numpy array references this parent
	);
};



template<class Type>
Multi_array<Type,3>::Multi_array
( 
	uint n_k , // Number of elements in j
	uint n_j , // Number of elements in j
	uint n_i , // Number of elements in i
	void* (*alloc_func)(size_t size) , // Custom allocation function 
	void (*free_func)(void* ptr)
)
: 
	alloc_func(alloc_func) , 
	free_func(free_func) , 
	ptr( (Type*)alloc_func( n_k*n_j*n_i*sizeof(Type)) ),
	n_k(n_k) , n_j(n_j) , n_i(n_i),
	stride_k(n_j*n_i*sizeof(Type)) , stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
	steps_k(n_j*n_i) , steps_j(n_i)
{};
	
template<class Type>
Multi_array<Type,3>::Multi_array( Type* ptr , uint n_k , uint n_j , uint n_i )
:
	alloc_func(NULL), /* No memory manegement allowed */
	free_func(NULL),
	ptr(ptr),
	n_k(n_k) , n_j(n_j) , n_i(n_i),
	stride_k(n_j*n_i*sizeof(Type)) , stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
	steps_k(n_j*n_i) , steps_j(n_i)
{};

template<class Type>
Multi_array<Type,3> Multi_array<Type,3>::numpy( py::array_t<Type, py::array::c_style> np_array )
{
	py::buffer_info buffer = np_array.request() ;
	
	if (buffer.ndim != 3) 
    {
		throw std::runtime_error("Number of dimensions must be two");
	}
	
	return Multi_array<Type,3>( (Type*)buffer.ptr , buffer.shape[0] , buffer.shape[1], buffer.shape[2] );
};

template<class Type>
Multi_array<Type,3>::~Multi_array()
{
	_free_func();
};

template<class Type>
void Multi_array<Type,3>::_free_func()
{
	
	if ( (free_func!=NULL) and (ptr!=NULL) )
	{ free_func(ptr) ; }
	ptr = NULL ;
};

// OPERATOR OVERLOAD 	
template<class Type>
inline Type& Multi_array<Type,3>::operator ()( uint k , uint j , uint i )
{
	return ptr[steps_k*k+steps_j*j+i] ;
};

template<class Type>
inline Type* Multi_array<Type,3>::operator()( uint k , uint j )
{
	return ptr + steps_k*k+steps_j*j ;
};

template<class Type>
inline Type* Multi_array<Type,3>::operator()( uint k )
{
	return ptr + steps_k*k ;
};

template<class Type>
inline Type* Multi_array<Type,3>::operator[]( uint k )
{
	return ptr + steps_k*k ;
};

template<class Type>
inline Type& Multi_array<Type,3>::operator ()( uint k , uint j , uint i ) const
{
	return ptr[steps_k*k+steps_j*j+i] ;
};

template<class Type>
inline Type* Multi_array<Type,3>::operator()( uint k , uint j ) const
{
	return ptr + steps_k*k+steps_j*j ;
};

template<class Type>
inline Type* Multi_array<Type,3>::operator()( uint k ) const
{
	return ptr + steps_k*k ;
};

template<class Type>
inline Type* Multi_array<Type,3>::operator[]( uint k ) const
{
	return ptr + steps_k*k ;
};
////

template<class Type>
Type* Multi_array<Type,3>::get()
{
	return ptr ;
};



template<class Type>
py::array_t<Type, py::array::c_style> Multi_array<Type,3>::get_py_copy()
{
	return py::array_t<Type, py::array::c_style>
	(
		{n_k,n_j,n_i},      // shape
		{stride_k,stride_j,stride_i},   // C-style contiguous strides for double
		ptr         // the data pointer
	);
};

template<class Type>
py::array_t<Type, py::array::c_style> Multi_array<Type,3>::get_py_no_copy()
{
	// Numpy will not copy the array when using the assignement operator=
	py::capsule free_dummy(	ptr, [](void *f){;} );
	
	return py::array_t<Type, py::array::c_style>
	(
		{n_k,n_j,n_i},      // shape
		{stride_k,stride_j,stride_i},   // C-style contiguous strides for double
		ptr ,       // the data pointer
		free_dummy // numpy array references this parent
	);
};

