// Constructor
template <class Type>
Buffered_Array<Type>::Buffered_Array(uint64_t L): 
	BufferDimension(L), BufferData( (Type*)calloc(L, sizeof(Type)) )
{
	// Memory on the heap is now manage by this class
}

// Destructor
template <class Type>
Buffered_Array<Type>::~Buffered_Array()
{
	free(BufferData); BufferData = NULL;
}

template <class Type>
void Buffered_Array<Type>::recallocate(uint64_t L)
{
	BufferDimension = L;
	BufferData = (Type*)realloc(BufferData, L*sizeof(Type));
	// set to 
	for(uint64_t i = 0; i<L ; i++)
	{
		BufferData[i] = 0 ;
	}
}

 












