// Constructor
template <class Type>
Buffered_Vector<Type>::Buffered_Vector(uint64_t L): 
	BufferDimension(L), BufferData( (Type*)calloc(L, sizeof(Type)) )
{
	// Memory on the heap is now manage by this class
}

// Destructor
template <class Type>
Buffered_Vector<Type>::~Buffered_Vector()
{
	free(BufferData); BufferData = NULL;
}

template <class Type>
void Buffered_Vector<Type>::recallocate(uint64_t L)
{
	BufferDimension = L;
	BufferData = (Type*)realloc(BufferData, L*sizeof(Type));
	// set to 0
	for(uint64_t i = 0; i<L ; i++)
	{
		BufferData[i] = 0 ;
	}
}

///// Buffered_Convolution ==>>>
// Constructor
template <class Type>
Buffered_Convolution<Type>::Buffered_Convolution(uint64_t l_data, uint64_t l_kernel): 
	 l_kernel(l_kernel), l_data(l_data), l_full(l_kernel + l_data - 1 ), l_valid(l_data - l_kernel + 1), data( (Type*)calloc(l_full, sizeof(Type)) ) , data_valid(data + l_kernel -1)
{
	// Memory on the heap is now manage by this class
}

// Destructor
template <class Type>
Buffered_Convolution<Type>::~Buffered_Convolution()
{
	free(data); data = NULL;
}

template <class Type>
void Buffered_Convolution<Type>::recallocate(uint64_t l_data, uint64_t l_kernel)
{
	l_data = l_data ;
	l_kernel = l_kernel;
	l_full = l_kernel + l_data - 1 ;
	l_valid = l_data - l_kernel + 1 ;
	data  = (Type*)realloc(data, l_full*sizeof(Type));
	data_valid = data + l_kernel -1 ; 
	// set to 0
	for(uint64_t i = 0; i<l_full ; i++)
	{
		data[i] = 0 ;
	}
}

 












