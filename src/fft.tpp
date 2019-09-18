////////////////////////////////////////////
//// Mother_FFT class
//Constructor
template <class FloatType, class DataType>
Mother_FFT<FloatType,DataType>::Mother_FFT
(   uint64_t L_data, uint32_t L_FFT , int nthreads ):
    L_FFT(L_FFT) , L_data(L_data) , nthreads(nthreads) , Time_alloc(0) 
   , Time_execution(0) , Time_preparation(0) 
   , p_data(NULL) , p_inplace_halfC(NULL) , p_direct(NULL)
{		
	cast_and_alloc();
	//omp_set_num_threads(nthreads);
}
// Destructor
template <class FloatType, class DataType>
Mother_FFT<FloatType,DataType>::~Mother_FFT()
{	  
	p_data = NULL ;
	p_inplace_halfC = NULL ; 
	p_direct = NULL ;
}

template <class FloatType, class DataType>
void Mother_FFT<FloatType,DataType>::cast_and_alloc()
{
	np_data.recallocate(L_data) ; 
	set_L_FFT( L_FFT );
	
	p_data = np_data.get_BufferData();
	p_inplace_halfC = np_inplace_halfC.get_BufferData();
	p_direct = np_direct.get_BufferData();
}

template <class FloatType, class DataType>
void Mother_FFT<FloatType,DataType>::set_L_FFT( uint32_t L_FFT )
{
	Timer Timer(&Time_alloc); // Times the duration of this scope
	
	this->L_FFT = L_FFT;
	np_inplace_halfC.recallocate( L_FFT/2 + 1 ) ; // enough space for half-complex transform 
	np_direct.recallocate(L_FFT) ;
    p_inplace_halfC = np_inplace_halfC.get_BufferData();
	p_direct = np_direct.get_BufferData();
}

template <class FloatType, class DataType>
void Mother_FFT<FloatType,DataType>::set_nthreads( int nthreads )
{
	this->nthreads = nthreads ;
	// omp_set_num_threads(nthreads);
}

////////////////////////////////////////////
////////////////////////////////////////////
//// Child class partial specialization

// Constructors
template <class DataType>
FFT<float,DataType>::FFT
(   uint64_t L_data, uint32_t L_FFT, int nthreads ): Mother_FFT<float,DataType>(L_data,L_FFT,nthreads)
{	
	fftwf_import_wisdom_from_filename("FFTWF_Wisdom.dat");
}
template <class DataType>
FFT<double,DataType>::FFT
(   uint64_t L_data, uint32_t L_FFT, int nthreads ): Mother_FFT<double,DataType>(L_data,L_FFT,nthreads)
{	
	fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
}

//////////
//Destructors
template <class DataType>
FFT<float,DataType>::~FFT()
{	  	
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
	fftwf_cleanup_threads();
}
template <class DataType>
FFT<double,DataType>::~FFT()
{	 	
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	fftw_cleanup_threads();
}
//////////
//////////prepare()
template <class DataType>
void FFT<float,DataType>::prepare()
{
	Timer Timer( & this->Time_preparation );
	fftwf_plan_with_nthreads( this->nthreads );
	plan = fftwf_plan_dft_r2c_1d( this->L_FFT , (float*)this->p_inplace_halfC , reinterpret_cast<fftwf_complex*>(this->p_inplace_halfC), FFTW_EXHAUSTIVE); // in-place fft
}
template <class DataType>
void FFT<double,DataType>::prepare()
{
	Timer Timer(& this->Time_preparation);
	fftw_plan_with_nthreads(  this->nthreads );
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
	Timer Timer(& this->Time_execution);
    for (uint64_t i = 0; i< 10000 ; i++)	
    {
        fftwf_execute(plan); 
    }
}
template <class DataType>
void FFT<double,DataType>::FFT_inplace()
{
	//Execute
	// fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
	// fftw_plan_with_nthreads(  this->nthreads );
	// plan = fftw_plan_dft_r2c_1d( this->L_FFT , (double*)this->p_inplace_halfC , reinterpret_cast<fftw_complex*>(this->p_inplace_halfC), FFTW_EXHAUSTIVE); // in-place fft
	Timer Timer(& this->Time_execution);
    for (uint64_t i = 0; i< 10000 ; i++)	
    {
        fftw_execute(plan); 
    }	
	// fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	// fftw_cleanup_threads();
}
//////////
////////////////////////////////////////////

