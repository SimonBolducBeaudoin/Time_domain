//////////////////////////////////////////////////////////////////////////////////////
//// 2D section
template <class IntType, class BinType>
Histogram2D_of_ints<IntType,BinType>::Histogram2D_of_ints( uint64_t NofBins , IntType* data_1 , IntType* data_2 , uint64_t L_data , std::string Algorithme , int n_threads )
:
    NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	p_historgam(NULL), 
	data_1(data_1), 
	data_2(data_2), 
	L_data(L_data)
{	
	omp_set_num_threads(n_threads);
	np_histogram.recallocate(NofBins) ; 
	p_historgam = np_histogram.get_BufferData();
}

template <class IntType, class BinType>
Histogram2D_of_ints<IntType,BinType>::Histogram2D_of_ints ( uint64_t NofBins , py::array_t<IntType> numpy_data_1, py::array_t<IntType> numpy_data_2  , std::string Algorithme , int n_threads )
:
	NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	p_historgam(NULL), 
	data_1(NULL), 
	data_2(NULL), 
	L_data(0)
{		
	omp_set_num_threads(n_threads);
	np_histogram.recallocate(NofBins) ; 
	p_historgam = np_histogram.get_BufferData();
	
	// Get the data from the numpy array
	py::buffer_info info_1 = numpy_data_1.request();
	py::buffer_info info_2 = numpy_data_2.request();
	
	if ( (info_1.format != py::format_descriptor<IntType>::format() )&& (info_2.format != py::format_descriptor<IntType>::format() ) )
	{
		throw std::runtime_error("Incompatible data type");
	}
	if ( (info_1.ndim != 1)&&(info_2.ndim != 1) )
	{
		throw std::runtime_error("Incompatible buffer dimension : expected 1D arrays.");
	}
	if ( numpy_data_1.size()!= numpy_data_2.size())
	{
		throw std::runtime_error(" Data vectors must be the same size.");
	}
	data_1 = (IntType*)numpy_data_1.ptr();
	data_2 = (IntType*)numpy_data_2.ptr();
	L_data = numpy_data_1.size();
	
}

template <class IntType, class BinType>
Histogram2D_of_ints<IntType,BinType>::Histogram2D_of_ints ( py::array_t<IntType> numpy_hist , py::array_t<IntType> numpy_data_1, py::array_t<IntType> numpy_data_2 , std::string Algorithme , int n_threads )
:
	NofBins(0) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	p_historgam(NULL), 
	data_1(NULL), 
	data_2(NULL), 
	L_data(0)
{		
	omp_set_num_threads(n_threads);
	
	// Get the data from the numpy array
	py::buffer_info info_1 = numpy_data_1.request();
	py::buffer_info info_2 = numpy_data_2.request();
	
	if ( (info_1.format != py::format_descriptor<IntType>::format() )&& (info_2.format != py::format_descriptor<IntType>::format() ) )
	{
		throw std::runtime_error("Incompatible data type");
	}
	if ( (info_1.ndim != 1)&&(info_2.ndim != 1) )
	{
		throw std::runtime_error("Incompatible buffer dimension : expected 1D arrays.");
	}
	if ( numpy_data_1.size()!= numpy_data_2.size())
	{
		throw std::runtime_error(" Data vectors must be the same size.");
	}
	data_1 = (IntType*)numpy_data_1.ptr();
	data_2 = (IntType*)numpy_data_2.ptr();
	L_data = numpy_data_1.size();
	
	// Get the histogram from the numpy array
	info_1 = numpy_hist.request();
	if (info_1.format != py::format_descriptor<BinType>::format())
	{
		throw std::runtime_error("Incompatible data type");
	}
	if (info_1.ndim != 2)
	{
		throw std::runtime_error("Incompatible buffer dimension : expected a 2D array.");
	}
	if (isPerfectSquare( numpy_hist.size() ))
	{
		throw std::runtime_error("Incompatible histogram shape: must be a square");
	}
	
	p_historgam = (BinType*)numpy_hist.ptr();
	NofBins = numpy_hist.shape(0);
	
	np_histogram.recallocate(0); // makes sure that the memory allocated for this is of size 0 and set BufferDimension = 0 ;
	np_histogram.set_BufferDimension(NofBins*NofBins) ; //  set BufferDimension = NofBins ;
	np_histogram.set_BufferData(p_historgam) ;
}

///////////////////////
///// METHODS : HISTOGRAMS OF INTEGERS

template <class IntType, class BinType>
void Histogram2D_of_ints<IntType,BinType>::set_n_threads( int n_threads )
{
	this->n_threads = n_threads ;
	omp_set_num_threads(n_threads);
}

// Same as in Histogram_int.tpp
template <class IntType, class BinType>
void Histogram2D_of_ints<IntType,BinType>::swap_histogram(BinType *hist, const int b)
{ 	
	const int halfL_data = 1<<(b-1);
    BinType* tmp = (BinType*) calloc(halfL_data, sizeof(BinType));
    int i=0;
    for (; i<halfL_data; i++)
	{  // Paralelizing those small loops is detrimental
        tmp[i] = hist[i];
        hist[i] = hist[i+halfL_data];
    }
    for (; i<2*halfL_data; i++)
	{
        hist[i] = tmp[i-halfL_data];
    }
    free(tmp);
}

template <class IntType, class BinType>
void Histogram2D_of_ints<IntType,BinType>::swap_histogram2D(BinType *hist, const int b)
{ 	
	uint64_t rL_data = 1<<b;     // Number AND Size of rows (because it's a square)
    swap_histogram(hist, 2*b); // Vertical swap
    #pragma omp parallel
    {
        manage_thread_affinity();
        #pragma omp for
        for (uint64_t i=0; i<rL_data; i++){ // For each row
            swap_histogram(hist+(i*rL_data), b); // Horizontal swap of each row
        }
    }
}

// Every object is going to have a copy of the following member functions.
// Although this is not ideal it's more readable than specializing functions for each child class.
template <class IntType, class BinType>
inline void Histogram2D_of_ints<IntType,BinType>::basic( uint8_t* data_1 , uint8_t* data_2 , uint64_t L_data, BinType* hist )
{	
	for (uint64_t i= 0 ; i<L_data; i++)
	{
        hist[ (data_1[i]<<8) + data_2[i] ]++;
    }
}
template <class IntType, class BinType>
inline void Histogram2D_of_ints<IntType,BinType>::basic( uint16_t* data_1 , uint16_t* data_2 , uint64_t L_data, BinType* hist, int b )
{	
	int tail = 16-b;
	uint16_t tmp1;
	uint16_t tmp2;
	
	for (uint64_t i=0 ; i<L_data; i++)
	{
		tmp1 = data_1[i];
		tmp2 = data_2[i];
        hist[ (( tmp1 >> tail ) << b) + (tmp2 >> tail) ]++; 
    }
}

template <class IntType, class BinType>
inline void Histogram2D_of_ints<IntType,BinType>::parallel( uint8_t* data_1 , uint8_t* data_2 , uint64_t L_data, BinType* hist )
{	
	uint64_t *data1_64 = (uint64_t *) data_1;
    uint64_t *data2_64 = (uint64_t *) data_2;
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        uint64_t tmp1=0;
        uint64_t tmp2=0;
        #pragma omp for reduction(+:hist[:1<<(8*2)])
        for (uint64_t i=0; i<L_data/8; i++){
            tmp1 = data1_64[i]; 
            tmp2 = data2_64[i]; 
            hist[ (tmp1 <<  8 & 0xFF00) + (tmp2 >>  0 & 0xFF) ]++;
            hist[ (tmp1 >>  0 & 0xFF00) + (tmp2 >>  8 & 0xFF) ]++;
            hist[ (tmp1 >>  8 & 0xFF00) + (tmp2 >> 16 & 0xFF) ]++;
            hist[ (tmp1 >> 16 & 0xFF00) + (tmp2 >> 24 & 0xFF) ]++;
            hist[ (tmp1 >> 24 & 0xFF00) + (tmp2 >> 32 & 0xFF) ]++;
            hist[ (tmp1 >> 32 & 0xFF00) + (tmp2 >> 40 & 0xFF) ]++;
            hist[ (tmp1 >> 40 & 0xFF00) + (tmp2 >> 48 & 0xFF) ]++;
            hist[ (tmp1 >> 48 & 0xFF00) + (tmp2 >> 56 & 0xFF) ]++;
        }
    }
    // The data that doesn't fit in 64bit chunks, openmp would be overkill here.
    for (uint64_t i=L_data-(L_data%8); i<L_data; i++){
        hist[ (data_1[i]<<8) + data_2[i] ]++;
    }
} 
template <class IntType, class BinType>
inline void Histogram2D_of_ints<IntType,BinType>::parallel( uint16_t* data_1 , uint16_t* data_2 , uint64_t L_data, BinType* hist, int b )
{	
	int tail0 = 16-b;
	int tail1 = tail0+16;
    int tail2 = tail1+16;
    int tail3 = tail2+16;
    int mask = (1<<b)-1; // Right amount of 0b1

    uint64_t *data1_64 = (uint64_t *) data_1;
    uint64_t *data2_64 = (uint64_t *) data_2;
	
	uint64_t **hs;
	int n;
	#pragma omp parallel
	{
		manage_thread_affinity(); // For 64+ logical cores on Windows
		n = omp_get_num_threads(); // Amount of threads
		
		#pragma omp single // Affects only next line
		hs = (uint64_t **) malloc(n * sizeof(uint64_t));
		uint64_t *h = (uint64_t *) calloc(1<<(b*2), sizeof(uint64_t)); // Filled with 0s.
		hs[omp_get_thread_num()] = h;
		
		uint64_t tmp1=0;
		uint64_t tmp2=0;
		#pragma omp for nowait
		for (uint64_t i=0; i<L_data/4; i++)
		{
			tmp1 = data1_64[i]; 
			tmp2 = data2_64[i]; 
			h[ ((tmp1 >> tail0 & mask) << b) + (tmp2 >> tail0 & mask) ]++;   
			h[ ((tmp1 >> tail1 & mask) << b) + (tmp2 >> tail1 & mask) ]++;
			h[ ((tmp1 >> tail2 & mask) << b) + (tmp2 >> tail2 & mask) ]++;
			h[ ((tmp1 >> tail3 & mask) << b) + (tmp2 >> tail3 & mask) ]++;
		}
	}
	// What didn't fit into chunks of 64
	uint16_t tmp1;
	uint16_t tmp2;
	for (uint64_t i=L_data/4 ; i<L_data/4+L_data%4; i++)
	{
        tmp1 = data_1[i];
		tmp2 = data_2[i];
        hist[ (( tmp1 >> tail0 ) << b) + (tmp2 >> tail0) ]++; 
    }
	// Critical reduction was very slow, this is faster.
	uint64_t NofBins_2D = 1<<(b*2);
	reduce(hs, NofBins_2D , 0, n); // hs[0] is the reduced array afterwards
	#pragma omp parallel
	{
		manage_thread_affinity();
		// Returning the result to the output array
		#pragma omp for
		for (uint64_t i=0; i<NofBins_2D; i++){
			hist[i]+=hs[0][i];
		}
	}
	for (int i=0; i<n; i++){
		free(hs[i]);
	}
	free(hs);
}
template <class IntType, class BinType>
inline void Histogram2D_of_ints<IntType,BinType>::atmc( uint16_t* data_1 , uint16_t* data_2 , uint64_t L_data, BinType* hist, int b )
{	
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
		// Full atomic should be faster when there's low memory collision, e.g. random data or large *b*.
		// Strikingly, for a given set of data it's typically faster for large *b*.
		// No local histogram; no reduction!
		#pragma omp for //reduction(+:h[:1<<(b*2)])  
		for (uint64_t i=0; i<L_data/4; i++){
			tmp1 = data1_64[i]; 
			tmp2 = data2_64[i]; 
			#pragma omp atomic update
			hist[ ((tmp1 >> tail0 & mask) << b) + (tmp2 >> tail0 & mask) ]++;   
			#pragma omp atomic update
			hist[ ((tmp1 >> tail1 & mask) << b) + (tmp2 >> tail1 & mask) ]++;
			#pragma omp atomic update
			hist[ ((tmp1 >> tail2 & mask) << b) + (tmp2 >> tail2 & mask) ]++;
			#pragma omp atomic update
			hist[ ((tmp1 >> tail3 & mask) << b) + (tmp2 >> tail3 & mask) ]++;
		}
	}
	// What didn't fit into chunks of 64
	uint16_t tmp1;
	uint16_t tmp2;
	for (uint64_t i=L_data/4 ; i<L_data/4+L_data%4; i++)
	{
        tmp1 = data_1[i];
		tmp2 = data_2[i];
        hist[ (( tmp1 >> tail0 ) << b) + (tmp2 >> tail0) ]++; 
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
///// METHODS SPECIFC TO DATATYPE
// UINT8 AND INT8 //////////////////////////////////////////////

template <class BinType>
void Histogram2D<uint8_t,BinType>::execute()
{	
	uint8_t* data_1 = this->data_1 ;
	uint8_t* data_2 = this->data_2 ;
	
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	
	if (this->Algorithme == "basic")
	{
		this->basic( data_1 , data_2 , L_data , hist );
	}
	else // default
	{
		this->parallel( data_1 , data_2 , L_data , hist );
	}
}

template <class BinType>
void Histogram2D<int8_t,BinType>::execute()
{
	int8_t* data_1 = this->data_1 ;
	int8_t* data_2 = this->data_2 ;
	
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	
	if (this->Algorithme == "basic")
	{
		this->basic( (uint8_t *)(data_1) , (uint8_t *)(data_2) , L_data , hist );
		this->swap_histogram2D( hist , 8); // b is always 8 here
	}
	else // default
	{
		this->parallel( (uint8_t *)( data_1 ) , (uint8_t *)(data_2)  , L_data , hist );
		this->swap_histogram2D( hist, 8); // b is always 8 here
	}
}

// UINT16 AND INT16 //////////////////////////////////////////////
template <class BinType>
void Histogram2D<uint16_t,BinType>::execute()
{
	uint16_t* data_1 = this->data_1 ;
	uint16_t* data_2 = this->data_2 ;
	
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	
	int b = this->sample_bit_length;
	
	if (this->Algorithme == "basic")
	{
		this->basic( data_1 , data_2 , L_data , hist  , b );
	}
	else if (this->Algorithme == "atomic")
	{
		this->atmc( data_1 , data_2 , L_data , hist , b );
	}
	else // default
	{
		this->parallel( data_1 , data_2 , L_data , hist , b );
	}
}

template <class BinType>
void Histogram2D<int16_t,BinType>::execute()
{
	int16_t* data_1 = this->data_1 ;
	int16_t* data_2 = this->data_2 ;
	
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	
	int b = this->sample_bit_length;
	
	if (this->Algorithme == "basic")
	{
		this->basic( (uint16_t *)(data_1) , (uint16_t *)(data_2) , L_data , hist , b );
		this->swap_histogram2D( hist , b ); 
	}
	else if (this->Algorithme == "atomic")
	{
		this->atmc( (uint16_t *)(this->data_1) , (uint16_t *)(this->data_2)  , this->L_data , this->p_historgam , this->sample_bit_length );
		this->swap_histogram2D(this->p_historgam , this->sample_bit_length); // b is always 8 here
	}
	else // default
	{
		this->parallel( (uint16_t *)(data_1) , (uint16_t *)(data_2)  , L_data , hist , b );
		this->swap_histogram2D( hist , b ); 
	}
}


