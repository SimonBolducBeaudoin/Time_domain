//////////////////////////////////////////////////////////////////////////////////////
//// 2D section
template <class FloatType, class BinType>
Histogram2D_of_floats<FloatType,BinType>::Histogram2D_of_floats ( uint64_t NofBins , FloatType* data_1 , FloatType* data_2 , uint64_t L_data , FloatType max , std::string Algorithme , int n_threads )
:
    NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) ,
	max(max), 
	bin_width(2*max/(this->NofBins)),
	p_historgam(NULL), 
	data_1(data_1), 
	data_2(data_2), 
	L_data(L_data)
{	
	omp_set_num_threads(n_threads);
	np_histogram.recallocate(NofBins) ; 
	p_historgam = np_histogram.get_BufferData();
}

template <class FloatType, class BinType>
Histogram2D_of_floats<FloatType,BinType>::Histogram2D_of_floats ( uint64_t NofBins , py::array_t<FloatType> numpy_data_1, py::array_t<FloatType> numpy_data_2 , FloatType max , std::string Algorithme , int n_threads )
:
	NofBins(NofBins) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	max(max), 
	bin_width(2*max/(this->NofBins)),
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
	
	if ( (info_1.format != py::format_descriptor<FloatType>::format() )&& (info_2.format != py::format_descriptor<FloatType>::format() ) )
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
	data_1 = (FloatType*)numpy_data_1.ptr();
	data_2 = (FloatType*)numpy_data_2.ptr();
	L_data = numpy_data_1.size();
	
	
}

template <class FloatType, class BinType>
Histogram2D_of_floats<FloatType,BinType>::Histogram2D_of_floats ( py::array_t<FloatType> numpy_hist , py::array_t<FloatType> numpy_data_1, py::array_t<FloatType> numpy_data_2 , FloatType max , std::string Algorithme , int n_threads )
:
	NofBins(0) , 
	n_threads(n_threads) , 
	Algorithme(Algorithme) , 
	max(max), 
	bin_width(2*max/(this->NofBins)),
	p_historgam(NULL), 
	data_1(NULL), 
	data_2(NULL), 
	L_data(0)
{		
	omp_set_num_threads(n_threads);
	
	// Get the data from the numpy array
	py::buffer_info info_1 = numpy_data_1.request();
	py::buffer_info info_2 = numpy_data_2.request();
	
	if ( (info_1.format != py::format_descriptor<FloatType>::format() )&& (info_2.format != py::format_descriptor<FloatType>::format() ) )
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
	data_1 = (FloatType*)numpy_data_1.ptr();
	data_2 = (FloatType*)numpy_data_2.ptr();
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
///// METHODS : HISTOGRAMS OF FLOATING POINTS

template <class FloatType, class BinType>
void Histogram2D_of_floats<FloatType,BinType>::set_n_threads( int n_threads )
{
	this->n_threads = n_threads ;
	omp_set_num_threads(n_threads);
}

template <class FloatType, class BinType>
inline void Histogram2D_of_floats<FloatType,BinType>::to_hist_middleman( uint8_t* h, BinType* hist, int bin )
{ 	
	if (h[bin]==255){
		hist[bin]+=(1<<8);
	}
	h[bin]++;
}

// Warning ! this function can only be used inside a parralel process 
template <class FloatType, class BinType>
inline void Histogram2D_of_floats<FloatType,BinType>::to_hist_middleman_pragma( uint8_t* h, BinType* hist, int bin )
{ 	
	if (h[bin]==255)
	{
		#pragma omp atomic update
		hist[bin]+=(1<<8);
	}
	h[bin]++;
}

template <class FloatType, class BinType>
inline void Histogram2D_of_floats<FloatType,BinType>::what_bin( const FloatType* data_1 , const FloatType* data_2, uint16_t* binx, uint16_t* biny )
{ 	
	FloatType data1 = *data_1;
	FloatType data2 = *data_2;
	uint64_t NofBins = this->NofBins;
	FloatType bin_width = this->bin_width;
	FloatType max = this->max;

	if (data1 >= max){
		// clipping
		*binx = NofBins-1;	
	}
	else if (data1 < -(max)){
		// clipping
		// add one to first bin
		*binx = 0;
	}
	else{
	*binx = (uint16_t)((data1+max)/(bin_width));
	}
	
	if (data2 >= max){
		// clipping
		*biny = NofBins-1;	
	}
	else if (data2 < -(max)){
		// clipping
		// add one to first bin
		*biny = 0;
	}
	else{
	*biny = (uint16_t)((data2+max)/(bin_width));
	}
}

template <class FloatType, class BinType>
inline void Histogram2D_of_floats<FloatType,BinType>::basic( const FloatType* data_1 , const FloatType* data_2, uint64_t L_data, int NofBins , BinType* hist )
{
	uint16_t binx = 0;
	uint16_t biny = 0;
	uint32_t bin;
	
	for (uint64_t i= 0; i< L_data; i++)
	{
		this->what_bin( data_1+i , data_2+i , &binx, &biny) ; 
		bin =  (binx * NofBins) + biny ;  
		hist[bin]++;
	}
}

template <class FloatType, class BinType>
inline void Histogram2D_of_floats<FloatType,BinType>::basic_middleman( const FloatType* data_1 , const FloatType* data_2, uint64_t L_data, int NofBins , BinType* hist )
{
	uint16_t binx = 0;
	uint16_t biny = 0;
	uint32_t bin;
	
	uint8_t* h = (uint8_t* ) calloc(NofBins*NofBins, sizeof(uint8_t));// Filled with 0s.

	for (uint64_t i= 0 ; i< L_data; i++)
	{
		what_bin( data_1+i , data_2+i , &binx, &biny ) ; 
		bin =  (binx * NofBins) + biny ;  
		to_hist_middleman( h, hist, bin );
	}
	
	// additionner le reste dans hist
	for(uint64_t i=0; i < NofBins*NofBins ; i++)
	{
		hist[i]+=h[i];
	}
	free(h);
}

////////////////////////////////////////////////////////////////////////////////////////////
///// METHODS SPECIFC TO FloatType

// FLOAT //////////////////////////////////////////////

template <class BinType>
void Histogram2D<float,BinType>::execute()
{
	int NofBins = this->NofBins ;
	
	float* data_1 = this->data_1 ;
	float* data_2 = this->data_2 ;
	uint64_t* p_data1_64 = (uint64_t *) data_1;
	uint64_t* p_data2_64 = (uint64_t *) data_2;
	
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	uint8_t n_threads = this->n_threads ; 
	
	if (this->Algorithme == "basic")
	{
		this->basic( data_1 , data_2, L_data, NofBins , hist );
	}
	else if (this->Algorithme == "basic_middleman")
	{
		this->basic_middleman( data_1 , data_2, L_data, NofBins , hist ); 
	}
	else if (this->Algorithme == "parallel_reduction")
	{	
		uint64_t **hs ;
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows
			
			#pragma omp single // Affects only next line
			hs = (uint64_t **) malloc(n_threads* sizeof(uint64_t));
			////
			uint64_t* h = (uint64_t* ) calloc( NofBins*NofBins , sizeof(uint64_t));// Filled with 0s.
			hs[omp_get_thread_num()] = h ;
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			#pragma omp for
			for (uint64_t i=0; i<L_data/2; i++)
			{
				this->what_bin( ((float*)(p_data1_64 + i) + 0) ,  ((float*)(p_data2_64 + i) + 0) , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				h[bin]++;
				this->what_bin( ((float*)(p_data1_64 + i) + 1) ,  ((float*)(p_data2_64 + i) + 1) , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				h[bin]++;
			}
		}
		// only 1 thread
		// Des fois il en reste 1
		for (uint64_t i= 2*(L_data/2); i< 2*(L_data/2) + L_data%2; i++)
		{
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			
			this->what_bin( data_1+i , data_2+i , &binx, &biny) ; 
			bin =  (binx * NofBins) + biny ;  
			hist[bin]++;
		}
		// Critical reduction was very slow, this is faster.
		reduce(hs, NofBins*NofBins, 0, n_threads); // hs[0] is the reduced array afterwards
		#pragma omp parallel
		{
			manage_thread_affinity();
			// Returning the result to the output array
			#pragma omp for
			for (uint64_t i=0; i<NofBins*NofBins; i++){
				hist[i]+=hs[0][i];
			}
		}
		for (int i=0; i<n_threads; i++){
			free(hs[i]);
		}
		free(hs);
	}
	else if (this->Algorithme == "atomic")
	{
		// Atomic makes the histogram in parralel but uses only one instance on hist in memory. 
		// It's slowed when two threads try to read and write in the same location in memory.
		// Therefoe it's faster when the histogram contains more bins (less colisions).
		// For some applications it could be faster to use a larger number of bin, then to concatenate neighbor
		// bins afterward (which is very low cost).
		
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			#pragma omp for
			for (uint64_t i=0; i<L_data/2; i++)
			{
				this->what_bin( ((float*)(p_data1_64 + i) + 0) ,  ((float*)(p_data2_64 + i) + 0) , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				#pragma omp atomic update
				hist[bin]++;
				this->what_bin( ((float*)(p_data1_64 + i) + 1) ,  ((float*)(p_data2_64 + i) + 1) , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				#pragma omp atomic update
				hist[bin]++;
			}
		}
		// Des fois il en reste 1
		for (uint64_t i= 2*(L_data/2); i< 2*(L_data/2) + L_data%2; i++)
		{
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			this->what_bin( data_1+i , data_2+i , &binx, &biny) ; 
			bin =  (binx * NofBins) + biny ;  
			hist[bin]++;
		}
		}
		// else if (this->Algorithme == "atomic_uint8")
		// {
			// atomic_uint8(); // The fastest when the number of bin is very high
		// }
		// else if (Algorithme == "squarred")
		// {
			 // Not Done yet
		// }
	else // default
	{
		uint8_t **hs;
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows

			#pragma omp single // Affects only next line
			hs = (uint8_t **) malloc(n_threads* sizeof(uint64_t));
			////
			uint8_t* h = (uint8_t* ) calloc( NofBins*NofBins, sizeof(uint8_t));// Filled with 0s.
			hs[omp_get_thread_num()] = h ;
			
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			#pragma omp for
			for (uint64_t i=0; i<L_data/2; i++)
			{
				this->what_bin( ((float*)(p_data1_64 + i) + 0) ,  ((float*)(p_data2_64 + i) + 0) , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ; 
				this->to_hist_middleman_pragma( h, hist, bin );
				// if (h[bin]==255)
				// {
					// #pragma omp atomic update
					// hist[bin]+=(1<<8);
				// }
				// h[bin]++;
				this->what_bin( ((float*)(p_data1_64 + i) + 0) ,  ((float*)(p_data2_64 + i) + 0) , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				this->to_hist_middleman_pragma( h, hist, bin );
				// if (h[bin]==255)
				// {
					// #pragma omp atomic update
					// hist[bin]+=(1<<8);
				// }
				// h[bin]++;
			}
		}
		// only 1 thread
		// Des fois il en reste 1
		for (uint64_t i= 2*(L_data/2); i< 2*(L_data/2) + L_data%2; i++)
		{
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			this->what_bin( data_1+i , data_2+i , &binx, &biny) ; 
			bin =  (binx * NofBins) + biny ;  
			hist[bin]++;
		}
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows
			for (uint64_t j=0; j<n_threads; j++)
			{
				#pragma omp for
				for (uint64_t i=0; i<(NofBins*NofBins); i++)
				{
					hist[i]+=hs[j][i];
				}
			}
		}
		for (int i=0; i<n_threads; i++){
			free(hs[i]);
		}
		free(hs);
	}
}


template <class BinType>
void Histogram2D<double,BinType>::execute()
{
	int NofBins = this->NofBins ;
	
	double* data_1 = this->data_1 ;
	double* data_2 = this->data_2 ;
	
	BinType* hist = this->p_historgam ;
	uint64_t L_data = this->L_data;
	uint8_t n_threads = this->n_threads ; 
	
	if (this->Algorithme == "basic")
	{
		this->basic( data_1 , data_2, L_data, NofBins , hist );
	}
	else if (this->Algorithme == "basic_middleman")
	{
		this->basic_middleman( data_1 , data_2, L_data, NofBins , hist ); 
	}
	else if (this->Algorithme == "parallel_reduction")
	{	
		uint64_t **hs ;
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows

			#pragma omp single // Affects only next line
			hs = (uint64_t **) malloc(n_threads* sizeof(uint64_t));
			////
			uint64_t* h = (uint64_t* ) calloc( NofBins*NofBins , sizeof(uint64_t));// Filled with 0s.
			hs[omp_get_thread_num()] = h ;
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			#pragma omp for
			for (uint64_t i=0; i<L_data; i++){
				this->what_bin( data_1 + i ,  data_2 + i , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				h[bin]++;
			}
		}
		// Critical reduction was very slow, this is faster.
		reduce(hs, NofBins*NofBins, 0, n_threads); // hs[0] is the reduced array afterwards
		#pragma omp parallel
		{
			manage_thread_affinity();
			// Returning the result to the output array
			#pragma omp for
			for (uint64_t i=0; i<NofBins*NofBins; i++){
				hist[i]+=hs[0][i];
			}
		}
		for (int i=0; i<n_threads; i++){
			free(hs[i]);
		}
		free(hs);
	}
	else if (this->Algorithme == "atomic")
	{
		// Atomic makes the histogram in parralel but uses only one instance on hist in memory. 
		// It's slowed when two threads try to read and write in the same location in memory.
		// Therefoe it's faster when the histogram contains more bins (less colisions).
		// For some applications it could be faster to use a larger number of bin, then to concatenate neighbor
		// bins afterward (which is very low cost).
		
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			#pragma omp for
			for (uint64_t i=0; i<L_data; i++)
			{
				this->what_bin( data_1 + i ,  data_2 + i , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				#pragma omp atomic update
				hist[bin]++;
			}
		}
	}
		// else if (this->Algorithme == "atomic_uint8")
		// {
			// atomic_uint8(); // The fastest when the number of bin is very high
		// }
		// else if (Algorithme == "squarred")
		// {
			 // Not Done yet
		// }
	else // default
	{
		uint8_t **hs;
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows

			#pragma omp single // Affects only next line
			hs = (uint8_t **) malloc(n_threads* sizeof(uint64_t));
			////
			uint8_t* h = (uint8_t* ) calloc( NofBins*NofBins, sizeof(uint8_t));// Filled with 0s.
			hs[omp_get_thread_num()] = h ;
			
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			#pragma omp for
			for (uint64_t i=0; i<L_data; i++){
				this->what_bin( data_1 + i ,  data_2 + i , &binx, &biny) ; 
				bin =  (binx * NofBins) + biny ;  
				if (h[bin]==255){
					#pragma omp atomic update
					hist[bin]+=(1<<8);
				}
				h[bin]++;
			}
		}
		// only 1 thread
		// Des fois il en reste 1
		for (uint64_t i= 2*(L_data); i< 2*(L_data) + L_data%2; i++)
		{
			uint16_t binx = 0;
			uint16_t biny = 0;
			uint32_t bin;
			this->what_bin( data_1+i , data_2+i , &binx, &biny) ; 
			bin =  (binx * NofBins) + biny ;  
			hist[bin]++;
		}
		#pragma omp parallel
		{
			manage_thread_affinity(); // For 64+ logical cores on Windows
			for (uint64_t j=0; j<n_threads; j++)
			{
				#pragma omp for
				for (uint64_t i=0; i<(NofBins*NofBins); i++)
				{
					hist[i]+=hs[j][i];
				}
			}
		}
		for (int i=0; i<n_threads; i++){
			free(hs[i]);
		}
		free(hs);
	}
}