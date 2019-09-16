//Constructors
template <class FloatType, class BinType>
Histogram_of_floats<FloatType,BinType>::Histogram_of_floats( uint64_t NofBins , FloatType* data , uint64_t L_data , FloatType max , std::string Algorithme , int n_threads )
:
    NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 	
	max(max), bin_width(2*max/(this->NofBins)),
	p_historgam(NULL), 
	data(data), 
	L_data(L_data)
{	
	omp_set_num_threads(n_threads);
	np_histogram.recallocate(NofBins) ; 
	p_historgam = np_histogram.get_BufferData();
}

template <class FloatType, class BinType>
Histogram_of_floats<FloatType,BinType>::Histogram_of_floats( uint64_t NofBins , py::array_t<FloatType> numpy_data, FloatType max , std::string Algorithme , int n_threads )
:
	NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	max(max), 
	bin_width(2*max/(this->NofBins)),
	p_historgam(NULL), 
	data(NULL), 
	L_data(0)
{	
	omp_set_num_threads(n_threads);
	np_histogram.recallocate(NofBins) ; 
	p_historgam = np_histogram.get_BufferData();
	
	// Get the data from the numpy array
	py::buffer_info info = numpy_data.request();
	if (info.format != py::format_descriptor<FloatType>::format())
	{
		throw std::runtime_error("Incompatible data type");
	}
	if (info.ndim != 1)
	{
		throw std::runtime_error("Incompatible buffer dimension : expected a 1D array.");
	}
	data = (FloatType*)numpy_data.ptr();
	L_data = numpy_data.size();
		
}

template <class FloatType, class BinType>
Histogram_of_floats<FloatType,BinType>::Histogram_of_floats( py::array_t<FloatType> numpy_hist , py::array_t<FloatType> numpy_data , FloatType max , std::string Algorithme , int n_threads )
:
	NofBins(0) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	max(max), 
	bin_width(2*max/(this->NofBins)),
	p_historgam(NULL), 
	data(NULL), 
	L_data(0)
{	
	omp_set_num_threads(n_threads);
	
	// Get the data for the numpy array
	py::buffer_info info = numpy_data.request();
	if (info.format != py::format_descriptor<FloatType>::format())
	{
		throw std::runtime_error("Incompatible data type");
	}
	if (info.ndim != 1)
	{
		throw std::runtime_error("Incompatible buffer dimension : expected a 1D array.");
	}
	data = (FloatType*)numpy_data.ptr();
	L_data = numpy_data.size();
	
	// Get the histogram from the numpy array
	info = numpy_hist.request();
	if (info.format != py::format_descriptor<BinType>::format())
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
///// METHODS : HISTOGRAMS OF FLOATING POINTS

template <class FloatType, class BinType>
void Histogram_of_floats<FloatType,BinType>::set_n_threads( int n_threads )
{
	this->n_threads = n_threads ;
	omp_set_num_threads(n_threads);
}

template <class FloatType, class BinType>
inline void Histogram_of_floats<FloatType,BinType>::to_hist(const FloatType* data, uint64_t L_data, BinType* hist )
{ 	
	FloatType tmp1 = *data;
	
	if (tmp1 >= this->max){
		// clipping
		hist[this->NofBins - 1]++; // add one to last bin 
	}
	else if (tmp1 < -(this->max)){
		// clipping
		hist[0]++; // add one to first bin
	}
	else{
	uint16_t bin = (uint16_t)((tmp1+max)/(this->bin_width));
	hist[ bin ]++;
	}	
}

template <class FloatType, class BinType>
inline void Histogram_of_floats<FloatType,BinType>::basic(const FloatType* data, uint64_t L_data, BinType* hist)
{ 	
	for (uint64_t i= 0 ; i< L_data; i++)
	{
		to_hist( (data+i), L_data , hist );	
	}
}

///////////////////////
///// METHODS SPECIFC TO FloatType

template <class BinType>
void Histogram<float,BinType>::execute()
{
	BinType* hist = this->p_historgam ;
	uint64_t N_bin = this-> NofBins;
	uint64_t L_data = this->L_data;
	float* data = this->data;
	uint64_t *data_64 = (uint64_t *) this->data;

	if (this->Algorithme == "basic")
	{
		this->basic(data, L_data, hist);
	}
	else // default : parallel_reduction
	{
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows
			#pragma omp for reduction(+:hist[:N_bin]) 
			for (uint64_t i=0; i<L_data/2; i++)
			{
				this->to_hist((float*)(data_64 + i) + 0, L_data , hist);
				this->to_hist((float*)(data_64 + i) + 1, L_data , hist);
			}
		}
		// The data that doesn't fit in 64bit chunks
		for (uint64_t i= 2*(L_data/2); i< 2*(L_data/2) + L_data%2; i++)
		{
			this->to_hist( (data+i), L_data , hist );	
		}
	}
}

template <class BinType>
void Histogram<double,BinType>::execute()
{
	BinType* hist = this->p_historgam ;
	uint64_t N_bin = this-> NofBins;
	uint64_t L_data = this->L_data;
	double* data = this->data;

	if (this->Algorithme == "basic")
	{
		this->basic(data, L_data, hist);
	}
	else // default : parallel_reduction
	{
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows
			#pragma omp for reduction(+:hist[:N_bin]) 
			for (uint64_t i=0; i<L_data; i++)
			{
				this->to_hist( (data+i) , L_data , hist);
			}
		}
	}
}
