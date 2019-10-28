// Constructor
template <class Type>
Numpy_Vector<Type>::Numpy_Vector(uint64_t L): 
	BufferDimension(L), BufferData( (Type*)calloc(L, sizeof(Type)) )
{
	// Memory on the heap is manage by this class
	info.ptr =	BufferData; 							/* Pointer to buffer */
	info.itemsize = sizeof(Type) ;						/* Size of one scalar */
	info.format = py::format_descriptor<Type>::value ;	/* Python struct-style format descriptor */
	info.ndim = 1 ; 									/* Number of dimensions */
	info.shape = { BufferDimension } ;					/* Buffer dimensions */
	info.strides = { sizeof(Type) }	;					/* Strides (in bytes) for each index */
	
	numpy_array( info );
}

// Destructor
template <class Type>
Numpy_Vector<Type>::~Numpy_Vector()
{ 
	free(BufferData);    
    BufferData = NULL ;
}

template <class Type>
void Numpy_Vector<Type>::recallocate(uint64_t L)
{
	BufferDimension = L;
	BufferData = (Type*)realloc(BufferData, L*sizeof(Type));
    
	for(uint64_t i = 0; i<L ; i++)
	{
		BufferData[i] = 0 ;
	}
	
	info.ptr =	BufferData ;
	info.shape = { BufferDimension } ;
}


 












