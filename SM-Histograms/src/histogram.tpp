template < class BinType >
Histogram<BinType>::Histogram( uint64_t nofbins, int n_threads )
:
	nofbins(nofbins) , 
	n_threads(n_threads),
	histogram(NULL)
{	
	Check_parity() ;
	Check_n_threads() ;
	
	omp_set_num_threads(n_threads);
	histogram = (BinType*)calloc(nofbins, sizeof(BinType));
} 

template < class BinType >
void Histogram<BinType>::Check_parity()
{
	if (nofbins%2 != 0 )
    {
		throw std::runtime_error(" The number of bin must be even.");
	}
}

template < class BinType >
void Histogram<BinType>::Check_n_threads()
{
	if ( n_threads <= 0 )
	{
		throw std::runtime_error(" n_threads <= 0 dont expect this to work... ");
	}
	else if ( n_threads > physical_n_threads() )
	{
		printf("Warning : The wanted number of thread (%d) is higher than the number of physical threads (%d) in this computer. n_thread was replaced by physical_n_threads. \n", n_threads, physical_n_threads() );
		n_threads = physical_n_threads();
	}
}


template < class BinType >
Histogram<BinType>::~Histogram()
{	
	if (histogram!=NULL){ free(histogram); }
	histogram = NULL;
} 

// ACCUMULATE METHODS 
template<class BinType>
void Histogram<BinType>::accumulate(  double* data,  uint64_t L_data , double max )
{
	// printf("Accumulating doubles \n");
	double bin_width = 2.0*max/( nofbins );
	// parallel_reduction
	#pragma omp parallel
	{
		manage_thread_affinity(); // For 64+ logical cores on Windows
		
		#pragma omp for reduction(+:histogram[:nofbins]) 
		for (uint64_t i=0; i<L_data; i++)
		{
			float_to_hist( data[i], histogram , max , bin_width );
		}
	}
}

/* template<class BinType>
void Histogram<BinType>::accumulate(  float* data,  uint64_t L_data , float max )
{
	// printf("Accumulating floats \n");
	uint64_t* data_64 = (uint64_t *) data;
	
	float bin_width = 2.0*max/( nofbins );
	// parallel_reduction
	#pragma omp parallel
	{
		manage_thread_affinity(); // For 64+ logical cores on Windows
		
		#pragma omp for reduction(+:histogram[:nofbins]) 
			for (uint64_t i=0; i<L_data/2; i++)
			{
				float_to_hist( ( (float*)(data_64 + i) )[0] , histogram , max , bin_width );
				float_to_hist( ( (float*)(data_64 + i) )[1] , histogram , max , bin_width );
			}
		// The data that doesn't fit in 64bit chunks
		for (uint64_t i= 2*(L_data/2); i< 2*(L_data/2) + L_data%2; i++)
		{
			float_to_hist( data[i], histogram , max , bin_width);	
		}
	}
} */

template<class BinType>
void Histogram<BinType>::accumulate( uint8_t* data,  uint64_t L_data )
{
	// printf("Accumulating uint8_ts \n");
	// Check if the histogram as the proper size
	if (nofbins != (1<<8))
	{
		throw std::runtime_error(" In order for Histogram of uint8_t to make sens nofbins must be equal to 256 .");
	}
	
	uint64_t *data_64 = (uint64_t *) data;
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        uint64_t tmp=0;
		// printf(" Hi from thread %d \n", omp_get_thread_num());
        #pragma omp for reduction(+:histogram[:1<<8])
        for (uint64_t i=0; i<L_data/8; i++)
		{
			// printf(" i = %llu, thread = %d \n", i ,omp_get_thread_num());
            tmp = data_64[i];
            histogram[tmp >>  0 & 0xFF]++;
            histogram[tmp >>  8 & 0xFF]++;
            histogram[tmp >> 16 & 0xFF]++;
            histogram[tmp >> 24 & 0xFF]++;
            histogram[tmp >> 32 & 0xFF]++;
            histogram[tmp >> 40 & 0xFF]++;
            histogram[tmp >> 48 & 0xFF]++;
            histogram[tmp >> 56 & 0xFF]++;
        }
    }
    // The data that doesn't fit in 64bit chunks, openmp would be overkill here.
    for (uint64_t i=L_data-(L_data%8); i<L_data; i++)
	{
        histogram[ data[i] ]++; 
    }
}

// Computes the histogram for (8<b<=16)-bit samples in uint16 containers
template<class BinType>
void Histogram<BinType>::accumulate( uint16_t* data,  uint64_t L_data , int b)
{
	// printf("Accumulating uint16_ts \n");
	if ( (b<=8)||(b>16) )
    {
		throw std::runtime_error(" b as to be in (8<b<=16)");
	}
	// Check if the histogram as the proper size
	if (nofbins != (uint64_t)(1<< b))
	{
		throw std::runtime_error(" In order for Histogram of uint16_t to make sens nofbins must be equal to 2**(b) .");
	}
	
	int tail = 16-b;
	uint64_t *data_64 = (uint64_t *) data;
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        uint64_t tmp=0;
        #pragma omp for reduction(+:histogram[:1<<b])
        for (uint64_t i=0; i<L_data/4; i++){
            tmp = data_64[i]; // tail get rid of bits > b
            histogram[ (tmp >>   0 & 0xFFFF) >> tail ]++;
            histogram[ (tmp >>  16 & 0xFFFF) >> tail ]++;
            histogram[ (tmp >>  32 & 0xFFFF) >> tail ]++;
            histogram[ (tmp >>  48 & 0xFFFF) >> tail ]++;
        }
    }
    // The data that doesn't fit in 64bit chunks, openmp would be overkill here.
    for (uint64_t i=L_data-(L_data%4); i<L_data; i++){
        histogram[ data[i] >> tail ]++; 
    }
}


////////////////////////////////////////////////

// float specific methods

template <class BinType>
template<class FloatType>
inline void Histogram<BinType>::float_to_hist( FloatType data, BinType* histogram , FloatType max , FloatType bin_width )
{ 	
	if ( data >= max )
	{
		// clipping
		histogram[ nofbins - 1 ]++; // add one to last bin 
	}
	else if ( data < -max )
	{
		// clipping
		histogram[0]++; // add one to first bin
	}
	else
	{
		histogram[ (uint64_t)((data+max)/(bin_width)) ]++;
	}	
}

/////////////////////////
// Histogram properties
/////////////////////////

template <class BinType>
double Histogram<BinType>::moment_k( uint k ,double first_bin_center , double bin_width )
{
    return ::moment_k(histogram, nofbins, k, first_bin_center , bin_width );
}

template <class BinType>
double Histogram<BinType>::moment_k_float( uint k ,double max )
{
    double bin_width = 2.0 * max/( nofbins );
    double first_bin_center = 0.5 * bin_width - max ;
    return ::moment_k(histogram, nofbins, k, first_bin_center , bin_width );
}

template <class BinType>
double Histogram<BinType>::centered_moment_k( uint k, double first_bin_center , double bin_width)
{
    return ::centered_moment_k(histogram, nofbins, k, first_bin_center , bin_width);
}

template <class BinType>
double Histogram<BinType>::centered_moment_k_float( uint k , double max)
{
    double bin_width = 2.0 * max/( nofbins );
    double first_bin_center = 0.5 * bin_width - max ;
    return ::centered_moment_k(histogram, nofbins, k, first_bin_center , bin_width);
}

template <class BinType>
double Histogram<BinType>::cumulant_k( uint k , double first_bin_center , double bin_width)
{
    return ::cumulant_k(histogram, nofbins, k, first_bin_center , bin_width);
}

template <class BinType>
double Histogram<BinType>::cumulant_k_float( uint k , double max)
{
    double bin_width = 2.0 * max/( nofbins );
    double first_bin_center = 0.5 * bin_width - max ;
    return ::cumulant_k(histogram, nofbins, k, first_bin_center , bin_width);
}

/*
## PYTHON WRAPPER METHODS
*/
/*
### accumulate METHODS
*/
template<class BinType>
template<class DataType>
void Histogram<BinType>::accumulate_float_py(  py::array_t<DataType> data, DataType max )
{
	py::buffer_info buf = data.request(); 

    if (buf.ndim != 1 )
    {
		throw std::runtime_error("Number of dimensions must be one");
	}
	
	uint64_t L_data = buf.size ;
	 
	accumulate( (DataType*)buf.ptr , L_data , max );
}

template<class BinType>
void Histogram<BinType>::accumulate_int_py( py::array_t<uint8_t> data )
{
	py::buffer_info buf = data.request(); 

    if (buf.ndim != 1 )
    {
		throw std::runtime_error("Number of dimensions must be one");
	}
	
	uint64_t L_data = buf.size ;
	 
	accumulate( (uint8_t*)buf.ptr , L_data );
}

template<class BinType>
void Histogram<BinType>::accumulate_int_py( py::array_t<uint16_t> data, int b )
{
	py::buffer_info buf = data.request(); 

    if (buf.ndim != 1 )
    {
		throw std::runtime_error("Number of dimensions must be one");
	}
	
	uint64_t L_data = buf.size ;
	 
	accumulate( (uint16_t*)buf.ptr , L_data, b );
}

/*
### get histogram
*/

template <class BinType>
py::array_t<BinType> Histogram<BinType>::get_py()
{
	/*
	Warning this will make a copy when using asignment operator= in pothon
	*/
	// Histogram class keeps ownership of the allocated memory for 
	// histogram. It will be freed only when the histogram object
	// will be garbaged collected in python.
	return py::array_t<BinType>
	(
		{nofbins,},      // shape
		{sizeof(BinType),},   // C-style contiguous strides for double
		histogram        // the data pointer
	);
}

/*
### get abscisse representation
*/

template <class BinType>
py::array_t<double> Histogram<BinType>::abscisse_float_py( double max )
{
	double bin_width = 2.0*max/( nofbins );
    
    double* abscisse = (double*)calloc(nofbins, sizeof(double));
    for(uint64_t i = 0; i < nofbins; i++)
    {
        abscisse[i] = ( (i + 0.5)*bin_width )- max ; 
    }
    
    py::capsule free_when_done (abscisse, [](void* f){double *abscisse = (double*)(f);free(abscisse);});
    
	return py::array_t<double>
	(
		{nofbins,},             // shape
		{sizeof(double),},      // C-style contiguous strides for double
		abscisse,               // the data pointer
        free_when_done          // Function to be called by the destructor to free memory
	);
}















