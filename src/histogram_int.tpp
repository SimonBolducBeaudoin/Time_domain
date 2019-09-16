//Constructors
template <class IntType, class BinType>
Histogram_of_ints<IntType,BinType>::Histogram_of_ints ( uint64_t NofBins , IntType* data , uint64_t L_data , std::string Algorithme , int n_threads )
:
    NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	p_historgam(NULL), 
	data(data), 
	L_data(L_data)
{	
	omp_set_num_threads(n_threads);
	np_histogram.recallocate(NofBins) ; 
	p_historgam = np_histogram.get_BufferData();
}

template <class IntType, class BinType>
Histogram_of_ints<IntType,BinType>::Histogram_of_ints ( uint64_t NofBins ,  py::array_t<IntType> numpy_data , std::string Algorithme , int n_threads )
:
	NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	p_historgam(NULL), 
	data(NULL), 
	L_data(0)
{		
	omp_set_num_threads(n_threads);
	np_histogram.recallocate(NofBins) ; 
	p_historgam = np_histogram.get_BufferData();
	
	// Get the data from the numpy array
	py::buffer_info info = numpy_data.request();
	if (info.format != py::format_descriptor<IntType>::format())
	{
		throw std::runtime_error("Incompatible data type");
	}
	if (info.ndim != 1)
	{
		throw std::runtime_error("Incompatible buffer dimension : expected a 1D array.");
	}
	data = (IntType*)numpy_data.ptr();
	L_data = numpy_data.size();
}

template <class IntType, class BinType>
Histogram_of_ints<IntType,BinType>::Histogram_of_ints ( py::array_t<IntType> numpy_hist , py::array_t<IntType> numpy_data, std::string Algorithme , int n_threads  )
:
	NofBins(0) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	p_historgam(NULL), 
	data(NULL), 
	L_data(0)
{		
	omp_set_num_threads(n_threads);
	
	// Get the data for the numpy array
	py::buffer_info info = numpy_data.request();
	if (info.format != py::format_descriptor<IntType>::format())
	{
		throw std::runtime_error("Incompatible data type");
	}
	if (info.ndim != 1)
	{
		throw std::runtime_error("Incompatible buffer dimension : expected a 1D array.");
	}
	data = (IntType*)numpy_data.ptr();
	L_data = numpy_data.size();
	
	// Get the histogram from the numpy array
	info = numpy_hist.request();
	if (info.format != py::format_descriptor<IntType>::format())
	{
		throw std::runtime_error("Incompatible data type");
	}
	if (info.ndim != 1)
	{
		throw std::runtime_error("Incompatible buffer dimension : expected a 1D array.");
	}
	p_historgam = (BinType*)numpy_hist.ptr();
	NofBins = numpy_hist.size();
	
	np_histogram.recallocate(0); // makes sure that the memory allocated for this is of size 0 and set BufferDimension = 0 ;
	np_histogram.set_BufferDimension(NofBins) ; //  set BufferDimension = NofBins ;
	np_histogram.set_BufferData(p_historgam) ;
}

///////////////////////
///// METHODS : HISTOGRAMS OF INTEGERS

template <class IntType, class BinType>
void Histogram_of_ints<IntType,BinType>::set_n_threads( int n_threads )
{
	this->n_threads = n_threads ;
	omp_set_num_threads(n_threads);
}

template <class IntType, class BinType>
void Histogram_of_ints<IntType,BinType>::swap_histogram(BinType *hist, const int b)
{ 	
	const int halfsize = 1<<(b-1);
    BinType* tmp = (BinType*) calloc(halfsize, sizeof(BinType));
    int i=0;
    for (; i<halfsize; i++)
	{  // Paralelizing those small loops is detrimental
        tmp[i] = hist[i];
        hist[i] = hist[i+halfsize];
    }
    for (; i<2*halfsize; i++)
	{
        hist[i] = tmp[i-halfsize];
    }
    free(tmp);
}

template <class IntType, class BinType>
inline void Histogram_of_ints<IntType,BinType>::basic(const uint8_t* data, uint64_t L_data, BinType* hist)
{ 	
	for (uint64_t i= 0 ; i< L_data; i++)
	{
		hist[ data[i] ]++;
	}
}

template <class IntType, class BinType>
inline void Histogram_of_ints<IntType,BinType>::basic(const uint16_t* data, uint64_t L_data, BinType* hist, int b )
{	
	int tail = 16-b;
	for (uint64_t i=0 ; i<L_data; i++){
        hist[ data[i] >> tail ]++; 
    }
}

template <class IntType, class BinType>
inline void Histogram_of_ints<IntType,BinType>::parallel( const uint8_t* data, uint64_t L_data, BinType* hist )
{	
	uint64_t *data_64 = (uint64_t *) data;
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        uint64_t tmp=0;
        #pragma omp for reduction(+:hist[:1<<8])
        for (uint64_t i=0; i<L_data/8; i++){
            tmp = data_64[i];
            hist[tmp >>  0 & 0xFF]++;
            hist[tmp >>  8 & 0xFF]++;
            hist[tmp >> 16 & 0xFF]++;
            hist[tmp >> 24 & 0xFF]++;
            hist[tmp >> 32 & 0xFF]++;
            hist[tmp >> 40 & 0xFF]++;
            hist[tmp >> 48 & 0xFF]++;
            hist[tmp >> 56 & 0xFF]++;
        }
    }
    // The data that doesn't fit in 64bit chunks, openmp would be overkill here.
    for (uint64_t i=L_data-(L_data%8); i<L_data; i++)
	{
        hist[ data[i] ]++; 
    }
}

template <class IntType, class BinType>
inline void Histogram_of_ints<IntType,BinType>::parallel( const uint16_t* data, uint64_t L_data, BinType* hist , int b)
{	
	int tail = 16-b;
	uint64_t *data_64 = (uint64_t *) data;
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        uint64_t tmp=0;
        #pragma omp for reduction(+:hist[:1<<b])
        for (uint64_t i=0; i<L_data/4; i++){
            tmp = data_64[i]; // tail get rid of bits > b
            hist[ (tmp >>   0 & 0xFFFF) >> tail ]++;
            hist[ (tmp >>  16 & 0xFFFF) >> tail ]++;
            hist[ (tmp >>  32 & 0xFFFF) >> tail ]++;
            hist[ (tmp >>  48 & 0xFFFF) >> tail ]++;
        }
    }
    // The data that doesn't fit in 64bit chunks, openmp would be overkill here.
    for (uint64_t i=L_data-(L_data%4); i<L_data; i++){
        hist[ data[i] >> tail ]++; 
    }
}

///////////////////////
///// METHODS SPECIFC TO DATATYPE
///////////// unint8 and int8

template <class BinType>
void  Histogram<uint8_t,BinType>::execute()
{
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	uint8_t* data = this->data;
	
	if (this->Algorithme == "basic")
	{
		this->basic( data, L_data, hist );
	}
	else // default : parallel_reduction
	{
		this->parallel( data, L_data, hist );
	}
}

template <class BinType>
void  Histogram<int8_t,BinType>::execute()
{
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	int8_t* data = this->data;
	
	if (this->Algorithme == "basic")
	{
		this->basic( (uint8_t *)data, L_data, hist );
		this->swap_histogram(this->p_historgam, 8); // b is always 8 here
	}
	else // default : parallel_reduction
	{
		this->parallel( (uint8_t *)(data) , L_data , hist );
		this->swap_histogram(this->p_historgam, 8); // b is always 8 here
	}
}

////////// unint16 and int16
template <class BinType>
void  Histogram<uint16_t,BinType>::execute()
{
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	uint16_t* data = this->data;
	
	if (this->Algorithme == "basic")
	{
		this->basic( data, L_data, hist, sample_bit_length );
	}
	else // default : parallel_reduction
	{
		this->parallel( data, L_data, hist, sample_bit_length );
	}
}

template <class BinType>
void  Histogram<int16_t,BinType>::execute()
{
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	int16_t* data = this->data;
	
	if (this->Algorithme == "basic")
	{
		this->basic( (uint16_t *)data, L_data, hist , sample_bit_length );
		this->swap_histogram(hist, sample_bit_length); // b is always 8 here
	}
	else // default : parallel_reduction
	{
		this->parallel( (uint16_t *)(data) , L_data , hist , sample_bit_length );
		this->swap_histogram( hist , sample_bit_length); // b is always 8 here
	}
}
