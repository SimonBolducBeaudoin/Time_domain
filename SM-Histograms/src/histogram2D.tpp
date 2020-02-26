template < class BinType >
Histogram2D<BinType>::Histogram2D( uint64_t nofbins, int n_threads )
:
	nofbins(nofbins) , 
	n_threads(n_threads),
	histogram(NULL)
{
	Check_parity() ;
	Check_n_threads() ;
	
	omp_set_num_threads(n_threads);
	
	Allocate_all_heap();
}

template < class BinType >
void Histogram2D<BinType>::Check_parity()
{
	if (nofbins%2 != 0 )
    {
		throw std::runtime_error(" The number of bin must be even.");
	}
}

template < class BinType >
void Histogram2D<BinType>::Check_n_threads()
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

template< class BinType>
void Histogram2D<BinType>::Allocate_all_heap()
{
	histogram = (BinType*)calloc(nofbins*nofbins, sizeof(BinType));
	hs = (uint8_t **) malloc(n_threads* sizeof(uint64_t));
	#pragma omp parallel
	{
		manage_thread_affinity(); // For 64+ logical cores on Windows
		hs[omp_get_thread_num()]  = (uint8_t* ) calloc( nofbins*nofbins, sizeof(uint8_t));// Filled with 0s.
	}
};

template< class BinType>
void Histogram2D<BinType>::Free_all_heap()
{
	#pragma omp parallel
	{ 
		free(hs[omp_get_thread_num()]) ; 
		hs[omp_get_thread_num()] = NULL ;
	}
	free(hs) ; 	hs = NULL;
	free(histogram); histogram = NULL;
};

template < class BinType >
Histogram2D<BinType>::~Histogram2D()
{	
	Free_all_heap();
} 

///////////////////////
///// METHODS : HISTOGRAMS OF FLOATING POINTS

// Warning ! this function can only be used inside a parralel process 
template <class BinType>
inline void Histogram2D<BinType>::to_hist_middleman_pragma( int this_thread , uint32_t bin )
{ 	
	// printf("bin = %u \n", bin);
	if (hs[this_thread][bin]==255)
	{
		#pragma omp atomic update
		histogram[bin]+=(1<<8);
	}
	hs[this_thread][bin]++;
}

template <class BinType>
template <class FloatType>
inline void Histogram2D<BinType>::what_bin_float( FloatType data_1 , FloatType data_2, FloatType max, FloatType bin_width, uint16_t* binx, uint16_t* biny )
{ 	
	if (data_1 >= max)
	{
		// clipping
		*binx = nofbins-1;	
	}
	else if (data_1 < -(max))
	{
		// clipping
		// add one to first bin
		*binx = 0;
	}
	else
	{
	*binx = (uint16_t)((data_1+max)/(bin_width));
	}
	if (data_2 >= max)
	{
		// clipping
		*biny = nofbins-1;	
	}
	else if (data_2 < -(max))
	{
		// clipping
		// add one to first bin
		*biny = 0;
	}
	else
	{
	*biny = (uint16_t)((data_2+max)/(bin_width));
	}
}

template <class BinType>
inline void Histogram2D<BinType>::reduction_and_reset()
{
	// Transfers the content of all smaller histograms into the main histogram
	#pragma omp parallel
	{
		manage_thread_affinity(); // For 64+ logical cores on Windows		
		
		// We avoid using # pragma omp atomic which as a time cost
		for (int j =0; j < n_threads; j++)
		{
			# pragma omp for
			for (uint64_t i=0; i<(nofbins*nofbins); i++)
			{
				histogram[i]+=hs[j][i];
				hs[j][i] = 0;
			}
		}	
	}
}


//////////////////////////
// ACCUMULATE METHODS
		
template < class BinType >
void Histogram2D<BinType>::accumulate( float* data_1, float* data_2,  uint64_t L_data, float max )
{
	// printf("Calling accumulate \n");
	float bin_width = 2*max/( nofbins );
	
	uint64_t* data_1_64 = (uint64_t *) data_1;
	uint64_t* data_2_64 = (uint64_t *) data_2;
	
	#pragma omp parallel
	{
		manage_thread_affinity(); // For 64+ logical cores on Windows

		uint16_t binx = 0;
		uint16_t biny = 0;
		uint32_t bin;
		int this_thread = omp_get_thread_num();
		
		#pragma omp for
		for (uint64_t i=0; i<L_data/2; i++)
		{
			what_bin_float( ((float*)(data_1_64 + i))[0] ,  ((float*)(data_2_64 + i))[0] , max, bin_width, &binx, &biny) ; 
			bin =  (binx * nofbins) + biny ;  
			to_hist_middleman_pragma( this_thread, bin );
			
			what_bin_float( ((float*)(data_1_64 + i))[1] ,  ((float*)(data_2_64 + i))[1] , max, bin_width, &binx, &biny) ; 
			bin =  (binx * nofbins) + biny ;  
			to_hist_middleman_pragma( this_thread, bin );
		}
	}
	
	// only 1 thread
	// Des fois il en reste 1
	for (uint64_t i= 2*(L_data/2); i< 2*(L_data/2) + L_data%2; i++)
	{
		uint16_t binx = 0;
		uint16_t biny = 0;
		uint32_t bin;
		
		what_bin_float( data_1[i] ,  data_2[i] , max, bin_width, &binx, &biny) ; 
		bin =  (binx * nofbins) + biny ;  
		histogram[bin]++;
	}
	
	// printf("reduction \n");
	reduction_and_reset();
}

template < class BinType >
void Histogram2D<BinType>::accumulate(  double* data_1, double* data_2,  uint64_t L_data, double max )
{
	// printf("Calling accumulate \n");
	double bin_width = 2.0*max/( nofbins );

	#pragma omp parallel
	{
		// printf("manage_thread_affinity \n");
		manage_thread_affinity(); // For 64+ logical cores on Windows
		
		uint16_t binx = 0;
		uint16_t biny = 0;
		uint32_t bin;
		int this_thread = omp_get_thread_num();
		
		// printf(" Hi from thread %d \n", omp_get_thread_num());
		#pragma omp for
		for (uint64_t i=0; i<L_data; i++)
		{
			// printf(" i = %llu, thread = %d \n", i ,omp_get_thread_num());
			what_bin_float( data_1[i] ,  data_2[i] , max, bin_width, &binx, &biny) ; 
			bin =  (binx * nofbins) + biny ;  
			to_hist_middleman_pragma( this_thread, bin );
		}
	}
	
	reduction_and_reset();
}


template < class BinType >
void Histogram2D<BinType>::accumulate(  uint8_t* data_1, uint8_t* data_2,  uint64_t L_data )
{
	// printf("accumulate<uint8_t>");
	// Check if the histogram as the proper size
	if (nofbins != (1<<8))
	{
		throw std::runtime_error(" In order for Histogram2D of uint8_t to make sens nofbins must be equal to 256 .");
	}
	uint64_t *data1_64 = (uint64_t *) data_1;
    uint64_t *data2_64 = (uint64_t *) data_2;
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        uint64_t tmp1=0;
        uint64_t tmp2=0;
		
		int this_thread = omp_get_thread_num();
        #pragma omp for 
        for (uint64_t i=0; i<L_data/8; i++)
		{
            tmp1 = data1_64[i]; 
            tmp2 = data2_64[i]; 
			
            to_hist_middleman_pragma( this_thread , (tmp1 <<  8 & 0xFF00) + (tmp2 >>  0 & 0xFF) ) ;
            to_hist_middleman_pragma( this_thread , (tmp1 >>  0 & 0xFF00) + (tmp2 >>  8 & 0xFF) ) ;
            to_hist_middleman_pragma( this_thread , (tmp1 >>  8 & 0xFF00) + (tmp2 >>  16 & 0xFF) ) ;
            to_hist_middleman_pragma( this_thread , (tmp1 >>  16 & 0xFF00) + (tmp2 >> 24 & 0xFF) ) ;
            to_hist_middleman_pragma( this_thread , (tmp1 >>  24 & 0xFF00) + (tmp2 >> 32 & 0xFF) ) ;
            to_hist_middleman_pragma( this_thread , (tmp1 >>  32 & 0xFF00) + (tmp2 >> 40 & 0xFF) ) ;
            to_hist_middleman_pragma( this_thread , (tmp1 >>  40 & 0xFF00) + (tmp2 >>  48 & 0xFF) ) ;
            to_hist_middleman_pragma( this_thread , (tmp1 >>  48 & 0xFF00) + (tmp2 >>  56 & 0xFF) ) ;
        }
    }
    // The data that doesn't fit in 64bit chunks, openmp would be overkill here.
    for (uint64_t i=L_data-(L_data%8); i<L_data; i++)
	{
        histogram[ (data_1[i]<<8) + data_2[i] ]++;
    }
	
	reduction_and_reset();
}

template < class BinType >
void Histogram2D<BinType>::accumulate(  uint16_t* data_1, uint16_t* data_2,  uint64_t L_data, int b )
{
	// printf("Accumulating uint16_ts \n");
	if ( (b<=8)||(b>16) )
    {
		throw std::runtime_error(" b as to be in (8<b<=16)");
	}
	// Check if the histogram as the proper size
	if (nofbins != (uint64_t)(1<<b))
	{
		throw std::runtime_error(" In order for Histogram2D of uint16_t to make sens nofbins must be equal to 2**b .");
	}
	
	int tail0 = 16-b;
	int tail1 = tail0+16;
    int tail2 = tail1+16;
    int tail3 = tail2+16;
    int mask = (1<<b)-1; // Right amount of 0b1

    uint64_t *data1_64 = (uint64_t *) data_1;
    uint64_t *data2_64 = (uint64_t *) data_2;
	
	#pragma omp parallel
	{
		manage_thread_affinity(); // For 64+ logical cores on Windows
			
		uint64_t tmp1=0;
		uint64_t tmp2=0;
		int this_thread = omp_get_thread_num();	
		#pragma omp for
		for (uint64_t i=0; i<L_data/4; i++)
		{
			tmp1 = data1_64[i]; 
			tmp2 = data2_64[i]; 
			
			to_hist_middleman_pragma( this_thread , ((tmp1 >> tail0 & mask) << b) + (tmp2 >> tail0 & mask) ) ;
			to_hist_middleman_pragma( this_thread , ((tmp1 >> tail1 & mask) << b) + (tmp2 >> tail1 & mask) ) ;
			to_hist_middleman_pragma( this_thread , ((tmp1 >> tail2 & mask) << b) + (tmp2 >> tail2 & mask) ) ;
			to_hist_middleman_pragma( this_thread , ((tmp1 >> tail3 & mask) << b) + (tmp2 >> tail3 & mask) ) ;
		}
	}
	// What didn't fit into chunks of 64
	uint16_t tmp1;
	uint16_t tmp2;
	for (uint64_t i=L_data/4 ; i<L_data/4+L_data%4; i++)
	{
        tmp1 = data_1[i];
		tmp2 = data_2[i];
        histogram[ (( tmp1 >> tail0 ) << b) + (tmp2 >> tail0) ]++; 
    }
	
	reduction_and_reset();	
}

/////////////////////////
// Histogram2D properties
/////////////////////////

template <class BinType>
double Histogram2D<BinType>::moment2D( uint exp_x , uint exp_y , double first_bin_center , double bin_width )
{
    return ::moment2D(histogram, nofbins, exp_x , exp_y , first_bin_center , bin_width );
}

template <class BinType>
double Histogram2D<BinType>::moment2D_float( uint exp_x , uint exp_y, double max )
{
    double bin_width = 2.0 * max/( nofbins );
    double first_bin_center = 0.5 * bin_width - max ;
    return ::moment2D(histogram, nofbins, exp_x , exp_y, first_bin_center , bin_width );
}

template <class BinType>
double Histogram2D<BinType>::cumulant_ii( double first_bin_center , double bin_width )
{
    return ::cumulant_ii(histogram, nofbins, first_bin_center , bin_width);
}

template <class BinType>
double Histogram2D<BinType>::cumulant_ii_float( double max )
{
    double bin_width = 2.0 * max/( nofbins );
    double first_bin_center = 0.5 * bin_width - max ;
    return ::cumulant_ii(histogram, nofbins, first_bin_center , bin_width);
}

template <class BinType>
double Histogram2D<BinType>::cumulant_jj( double first_bin_center , double bin_width )
{
    return ::cumulant_jj(histogram, nofbins, first_bin_center , bin_width);
}

template <class BinType>
double Histogram2D<BinType>::cumulant_jj_float( double max )
{
    double bin_width = 2.0 * max/( nofbins );
    double first_bin_center = 0.5 * bin_width - max ;
    return ::cumulant_jj(histogram, nofbins, first_bin_center , bin_width);
}

template <class BinType>
double Histogram2D<BinType>::cumulant_iijj( double first_bin_center , double bin_width )
{
    return ::cumulant_iijj(histogram, nofbins, first_bin_center , bin_width);
}

template <class BinType>
double Histogram2D<BinType>::cumulant_iijj_float( double max )
{
    double bin_width = 2.0 * max/( nofbins );
    double first_bin_center = 0.5 * bin_width - max ;
    return ::cumulant_iijj(histogram, nofbins, first_bin_center , bin_width);
}

/////////////////////////
// PYTHON WRAPPER METHODS
/////////////////////////
template <class BinType>
py::array_t<BinType> Histogram2D<BinType>::get_py()
{
	// Histogram2D class keeps ownership of the allocated memory for 
	// histogram. It will be freed only when the histogram object
	// will be garbaged collected in python.
	return py::array_t<BinType>
	(
		{nofbins,nofbins,},      // shape
		{nofbins*sizeof(BinType),sizeof(BinType),},   // C-style contiguous strides for double
		histogram        // the data pointer
	);
}

template<class BinType>
template<class FloatType>
void Histogram2D<BinType>::accumulate_float_py(  py::array_t<FloatType> data_1, py::array_t<FloatType> data_2, FloatType max )
{
	// printf("Calling accumulate_float_py \n");
    ////////////////////////////////
    // Timer Timer(& accumulate_timer) ; 
    ////////////////////////////////
	py::buffer_info buf_1 = data_1.request(),
					buf_2 =	data_2.request(); 

    if ( (buf_1.ndim != 1)||(buf_2.ndim != 1) )
    {
		throw std::runtime_error("Number of dimensions must be one");
	}
	if ( buf_1.size !=  buf_2.size )
    {	
		throw std::runtime_error("Length of the data vectors must be the same.");
	}
	
	uint64_t L_data = buf_1.size ;
	// printf("L_data = %llu \n", L_data);
	 
	accumulate( (FloatType*)buf_1.ptr, (FloatType*)buf_2.ptr , L_data , max );
}

template<class BinType>
void Histogram2D<BinType>::accumulate_int_py(  py::array_t<uint8_t> data_1, py::array_t<uint8_t> data_2 )
{
	// printf("Calling accumulate_int_py \n");
	py::buffer_info buf_1 = data_1.request(),
					buf_2 =	data_2.request(); 

    if ( (buf_1.ndim != 1)||(buf_2.ndim != 1) )
    {
		throw std::runtime_error("Number of dimensions must be one");
	}
	if ( buf_1.size !=  buf_2.size )
    {	
		throw std::runtime_error("Length of the data vectors must be the same.");
	}
	
	uint64_t L_data = buf_1.size ;
	// printf("L_data = %llu \n", L_data);
	 
	accumulate( (uint8_t*)buf_1.ptr, (uint8_t*)buf_2.ptr , L_data );
}

template<class BinType>
void Histogram2D<BinType>::accumulate_int_py(  py::array_t<uint16_t> data_1, py::array_t<uint16_t> data_2 , int b)
{
	// printf("Calling accumulate_int_py \n");
	py::buffer_info buf_1 = data_1.request(),
					buf_2 =	data_2.request(); 

    if ( (buf_1.ndim != 1)||(buf_2.ndim != 1) )
    {
		throw std::runtime_error("Number of dimensions must be one");
	}
	if ( buf_1.size !=  buf_2.size )
    {	
		throw std::runtime_error("Length of the data vectors must be the same.");
	}
	
	uint64_t L_data = buf_1.size ;
	// printf("L_data = %llu \n", L_data);
	 
	accumulate( (uint16_t*)buf_1.ptr, (uint16_t*)buf_2.ptr , L_data , b );
}

template <class BinType>
py::array_t<double> Histogram2D<BinType>::abscisse_float_py( double max )
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