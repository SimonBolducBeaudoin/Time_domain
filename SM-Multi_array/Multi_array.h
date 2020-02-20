#pragma once
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
void* _128bit_aligned_malloc( size_t size );


/////// FUTURE IDEA
	// Add a constructor with custom strides values to account for cases : 
	// Multi_array<complex_d,2>(n, L//2 + 1 )
	// 	Stride are ( n*-> , (L//2 + 1) *(128) )
	// Multi_array<double,2>(n, L ) // 8 bytes less per line
	// 	Keep the same strides but reinterpretcast each line
	
	// Add a macro that can be turned on and off that checks if called indexes are out of bounds.


template<class Type, uint8_t Dim>
class Multi_array
{
	/*
	Custom class for runtime allocation of fixed size multidimentionnal arrays.
		- Memory is going to be allocated to be maximaly local, with index logic beeing indexes(uint lless_local, less_local, local)
			- Therefore fastest iterating loop must always be iterating on last index for optimal performance.
		- Memory will be aligned be default : 
			https://embeddedartistry.com/blog/2017/02/22/generating-aligned-memory/
		- which size are known only at runtime but are of fixed size.
		- Dim (the number of dimension) has to be declared at compile time.
		- (Optional not implemented yet) The constructor takes memory allocation and free function pointers (ex for use with fftw's fftw_alloc and fftw_free)
	*/
};

template<class Type>
class Multi_array<Type,1>
{
	public :
	/* usual constructor */
	Multi_array 
	(
		uint n_i , // Number of elements in i
		void* (*alloc_func)(size_t size) = &_128bit_aligned_malloc, // Custom allocation function 
		void (*free_func)(void* ptr) = &_aligned_free
	);
	
	/* Constructing from an existing pointer */
	Multi_array ( Type* prt, uint n_i );
	
	/* Constructing from a 1D Numpy array */
	static Multi_array numpy( py::array_t<Type,py::array::c_style> np_array );
	
	/* Copy constructor */
	Multi_array( Multi_array& Mom)
	:
		alloc_func(NULL), /* No memory manegement allowed */
		free_func(NULL),
		ptr(Mom.ptr),
		n_i(Mom.n_i),
		stride_i(sizeof(Type))
	{};
	
	Multi_array(const Multi_array& Mom)
	:
		alloc_func(NULL), /* No memory manegement allowed */
		free_func(NULL),
		ptr(Mom.ptr),
		n_i(Mom.n_i),
		stride_i(sizeof(Type))
	{};
	
	~Multi_array();
	
	Type& operator()( uint i ); /* Returns a reference to an element */
	Type& operator[]( uint i ); /* Returns a reference to an element */
	
	/* 
		Same behavior for const Multi_array
		See : https://en.cppreference.com/w/cpp/language/member_functions#const-_and_volatile-qualified_member_functions
	*/
	Type& operator()( uint i ) const ; /* Returns a reference to an element */
	Type& operator[]( uint i ) const ; /* Returns a reference to an element */
	
	
	Type* get();
	py::array_t<Type, py::array::c_style> get_py_copy();
	py::array_t<Type, py::array::c_style> get_py_no_copy();
	
	uint get_n_i(){return n_i;};
	
	private :
	void* (*alloc_func)(size_t size) ;
	void (*free_func)(void* ptr) ;
	
	Type* ptr ;
	const uint  n_i ;
	size_t stride_i ;
	
	void _free_func();
	
};

template<class Type>
class Multi_array<Type,2>
{
	public :
	
	Multi_array
	( 
		uint n_j , // Number of elements in j
		uint n_i , // Number of elements in i
		void* (*alloc_func)(size_t size) = &_128bit_aligned_malloc, // Custom allocation function 
		void (*free_func)(void* ptr) = &_aligned_free
	);
	
	/* Constructing from an existing pointer */
	Multi_array
	(
		Type* prt,
		uint n_j,
		uint n_i
	);
	
	/* Constructing from a 2D Numpy array */
	static Multi_array numpy( py::array_t<Type,py::array::c_style> np_array );
	
	/* Copy constructor */
	Multi_array( Multi_array& Mom)
	:
		alloc_func(NULL), /* No memory manegement allowed */
		free_func(NULL),
		ptr(Mom.ptr),
		n_j(Mom.n_j), n_i(Mom.n_i),
		stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
		steps_j(n_i)
	{
		printf("line number %d in file %s\n", __LINE__, __FILE__);
	};
	
	Multi_array(const Multi_array& Mom)
	:
		alloc_func(NULL), /* No memory manegement allowed */
		free_func(NULL),
		ptr(Mom.ptr),
		n_j(Mom.n_j), n_i(Mom.n_i),
		stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
		steps_j(n_i)
	{
		printf("line number %d in file %s\n", __LINE__, __FILE__);
	};
	
	~Multi_array();
	
	Type& operator()( uint j , uint i ); /* Returns a reference to an element */
	Type* operator()( uint j ) ; /* Returns a pointer to a row */ 
	Type* operator[]( uint j ) ; /* Returns a pointer to a row */ 
	
	/* Same behavior for const Multi_array	*/
	Type& operator()( uint j , uint i ) const ; /* Returns a reference to an element */
	Type* operator()( uint j ) const ; /* Returns a pointer to a row */ 
	Type* operator[]( uint j ) const ; /* Returns a pointer to a row */ 
	
	Type* get();
	py::array_t<Type, py::array::c_style> get_py_copy();
	py::array_t<Type, py::array::c_style> get_py_no_copy();
	
	uint get_n_i(){return n_i;};
	uint get_n_j(){return n_j;};
	
	private :
	void* (*alloc_func)(size_t size) ;
	void (*free_func)(void* ptr) ;
	
	Type* ptr ;
	const uint n_j ;
	const uint n_i ;
	
	size_t stride_j ;
	size_t stride_i ;
	
	uint steps_j ; /* Has the same meaning as stride/siseof(Type) */
	
	void _free_func();
};

template<class Type>
class Multi_array<Type,3>
{
	public :
	Multi_array
	( 
		uint n_k , // Number of elements in k
		uint n_j , // Number of elements in j
		uint n_i , // Number of elements in i
		void* (*alloc_func)(size_t size) = &_128bit_aligned_malloc, // Custom allocation function 
		void (*free_func)(void* ptr) = &_aligned_free
	);
	
	/* Constructing from an existing pointer */
	Multi_array
	(
		Type* prt,
		uint n_k,
		uint n_j,
		uint n_i
	);
	
	/* Constructing from a 3D Numpy array */
	static Multi_array numpy( py::array_t<Type,py::array::c_style> np_array );
	
	/* Copy constructor */
	Multi_array( Multi_array& Mom)
	:
		alloc_func(NULL), /* No memory manegement allowed */
		free_func(NULL),
		ptr(Mom.ptr),
		n_k(Mom.n_k) , n_j(Mom.n_j) , n_i(Mom.n_i),
		stride_k(n_j*n_i*sizeof(Type)) , stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
		steps_k(n_j*n_i) , steps_j(n_i)
	{
		printf("line number %d in file %s\n", __LINE__, __FILE__);
	};
	
	Multi_array(const Multi_array& Mom)
	:
		alloc_func(NULL), /* No memory manegement allowed */
		free_func(NULL),
		ptr(Mom.ptr),
		n_k(Mom.n_k) , n_j(Mom.n_j) , n_i(Mom.n_i),
		stride_k(n_j*n_i*sizeof(Type)) , stride_j(n_i*sizeof(Type)) , stride_i(sizeof(Type)),
		steps_k(n_j*n_i) , steps_j(n_i)
	{
		printf("line number %d in file %s\n", __LINE__, __FILE__);
	};
	
	~Multi_array();
	
	Type& operator()( uint k , uint j , uint i ); /* Returns a reference to an element */
	Type* operator()( uint k , uint j  ); /* Returns a pointer to kj'th row */
	Type* operator()( uint k ) ; /* Returns a pointer to the k'th 2D subarray */ 
	Type* operator[]( uint k ) ; /* Returns a pointer to the k'th 2D subarray */ 
	
	/* Same behavior for const Multi_array	*/
	Type& operator()( uint k , uint j , uint i ) const ; /* Returns a reference to an element */
	Type* operator()( uint k , uint j  ) const ; /* Returns a pointer to kj'th row */
	Type* operator()( uint k ) const ; /* Returns a pointer to the k'th 2D subarray */ 
	Type* operator[]( uint k ) const ; /* Returns a pointer to the k'th 2D subarray */ 
	
	Type* get();
	py::array_t<Type, py::array::c_style> get_py_copy();
	py::array_t<Type, py::array::c_style> get_py_no_copy();
	
	uint get_n_i(){return n_i;};
	uint get_n_j(){return n_j;};
	uint get_n_k(){return n_k;};
	
	private :
	void* (*alloc_func)(size_t size) ;
	void (*free_func)(void* ptr) ;
	
	Type* ptr ;
	const uint n_k ; 
	const uint n_j ;
	const uint n_i ;
	
	size_t stride_k ;
	size_t stride_j ;
	size_t stride_i ;
	
	uint steps_k ; /* Has the same meaning as stride/siseof(Type) */
	uint steps_j ;
	
	void _free_func();
};

