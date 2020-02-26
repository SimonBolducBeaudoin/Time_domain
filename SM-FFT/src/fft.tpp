////////////////////////////////////////////
//// Mother_FFT class
//Constructor
template <class FloatType, class DataType>
Mother_FFT<FloatType,DataType>::Mother_FFT
(   uint64_t L_data, uint32_t L_FFT , int n_threads ):
    L_FFT(L_FFT) , L_data(L_data) , n_threads(n_threads) 
	, p_data(NULL) , p_inplace_halfC(NULL) , p_direct(NULL)
{		
	omp_set_num_threads(n_threads);
}
// Destructor
template <class FloatType, class DataType>
Mother_FFT<FloatType,DataType>::~Mother_FFT()
{	  
	p_data = NULL ;
	p_inplace_halfC = NULL ; 
	p_direct = NULL ;
}

////////////////////////////////////////////
////////////////////////////////////////////
//// Child class partial specialization

// Constructors
template <class DataType>
FFT<float,DataType>::FFT
(   uint64_t L_data, uint32_t L_FFT, int n_threads ): Mother_FFT<float,DataType>(L_data,L_FFT,n_threads)
{	
	fftwf_import_wisdom_from_filename("FFTWF_Wisdom.dat");
	prepare();
}
template <class DataType>
FFT<double,DataType>::FFT
(   uint64_t L_data, uint32_t L_FFT, int n_threads ): Mother_FFT<double,DataType>(L_data,L_FFT,n_threads)
{	
	fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
	prepare();
}

//////////
//Destructors
template <class DataType>
FFT<float,DataType>::~FFT()
{	  	
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
	// fftwf_cleanup_threads();
}
template <class DataType>
FFT<double,DataType>::~FFT()
{	 	
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	// fftw_cleanup_threads();
}
//////////
//////////set_L_FFT()
template <class DataType>
void FFT<float,DataType>::set_L_FFT( uint32_t L_FFT )
{
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
	
	this->L_FFT = L_FFT;

	free_heap();
	prepare();
}
template <class DataType>
void FFT<double,DataType>::set_L_FFT( uint32_t L_FFT )
{
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	
	this->L_FFT = L_FFT;
	
	free_heap();
	prepare();
}

//////////set_n_threads()
template <class DataType>
void FFT<float,DataType>::set_n_threads( int n_threads )
{
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");

	free_heap();
	
	this->n_threads = n_threads ;
	omp_set_num_threads( n_threads ) ; 

	prepare();
}

template <class DataType>
void FFT<double,DataType>::set_n_threads( int n_threads )
{
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");

	free_heap();
	
	this->n_threads = n_threads ;
	omp_set_num_threads( n_threads ) ; 

	prepare();
}
//////////
////////// free_heap()
template <class DataType>
void FFT<float,DataType>::free_heap()
{
	// fftwf_free(kernel_complex); kernel_complex = NULL ;
	
	// for(int i=0; i< this->n_threads; i++)
	// {
		// fftwf_free (gs[i]); gs[i] = NULL ;
		// fftwf_free (hs[i]); hs[i] = NULL ;
	// }
    // free(gs); gs = NULL ;
    // free(hs); hs = NULL ;
}

template <class DataType>
void FFT<double,DataType>::free_heap()
{
	// fftw_free(kernel_complex); kernel_complex = NULL ;
	
	// for(int i=0; i< this->n_threads; i++)
	// {
		// fftw_free (gs[i]); gs[i] = NULL ;
		// fftw_free (hs[i]); hs[i] = NULL ;
	// }
    // free(gs); gs = NULL ;
    // free(hs); hs = NULL ;
}

//////////prepare()
template <class DataType>
void FFT<float,DataType>::prepare()
{
	this->np_data.recallocate(this->L_data) ; 
	this->p_data = this->np_data.get_BufferData();
	// np_data = py::array_t<DataType , py::array::c_style>  ; 
	
	
	this->np_inplace_halfC.recallocate( this->L_FFT/2 + 1 ) ;
	this->np_direct.recallocate(this->L_FFT) ;
	
    this->p_inplace_halfC = this->np_inplace_halfC.get_BufferData();
	this->p_direct = this->np_direct.get_BufferData();
	
	// fftwf_plan_with_n_threads( this->n_threads );
	plan = fftwf_plan_dft_r2c_1d( this->L_FFT , (float*)this->p_inplace_halfC , reinterpret_cast<fftwf_complex*>(this->p_inplace_halfC), FFTW_EXHAUSTIVE); // in-place fft
}
template <class DataType>
void FFT<double,DataType>::prepare()
{
	this->np_data.recallocate(this->L_data) ; 
	this->np_inplace_halfC.recallocate( this->L_FFT/2 + 1 ) ;
	this->np_direct.recallocate(this->L_FFT) ;
	
	this->p_data = this->np_data.get_BufferData();
    this->p_inplace_halfC = this->np_inplace_halfC.get_BufferData();
	this->p_direct = this->np_direct.get_BufferData();
	
	// fftw_plan_with_n_threads(  this->n_threads );
	plan = fftw_plan_dft_r2c_1d( this->L_FFT , (double*)this->p_inplace_halfC , reinterpret_cast<fftw_complex*>(this->p_inplace_halfC), FFTW_EXHAUSTIVE); // in-place fft
}
//////////
//////////execute()
template <class DataType>
void FFT<float,DataType>::execute()
{
	// conditionnals
	FFT_inplace();
	//end
}
template <class DataType>
void FFT<double,DataType>::execute()
{
	// conditionnals
	FFT_inplace();
	//end
}
//////////
//////////FFT_inplace()
template <class DataType>
void FFT<float,DataType>::FFT_inplace()
{
    fftwf_execute(plan); 
}
template <class DataType>
void FFT<double,DataType>::FFT_inplace()
{
	fftw_execute(plan); 
}
//////////
////////////////////////////////////////////

