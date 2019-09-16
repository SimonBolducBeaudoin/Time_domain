//Constructor
template <class FloatType, class DataType>
Time_Quad<FloatType,DataType>::Time_Quad
(	
	uint64_t L_kernel,  uint64_t L_data, uint32_t L_FFT, FloatType dt, std::string Convolution_type
):
	L_kernel(L_kernel) , L_data(L_data) , L_FFT(L_FFT) , dt(dt), Convolution_type(Convolution_type),
	p_kernel_p(NULL), p_kernel_q(NULL), p_data(NULL), p_p(NULL), p_q(NULL)
{
	L_full = L_kernel + L_data - 1 ;
	L_valid = L_data - L_kernel + 1 ;
	
	set_L_FFT( L_FFT );
	
	// Checks for errors cast type and allocate memory for pointers and declare numpy arrays.
	checks();
	cast_and_alloc();
	// Asign kernels values
	make_kernels();
	
}

//Destructor
template <class FloatType, class DataType>
Time_Quad<FloatType,DataType>::~Time_Quad()
{	 
	p_kernel_p = NULL;
	p_kernel_q = NULL;
	p_p = NULL;
	p_q = NULL; 
	p_data = NULL;
}

template <class FloatType, class DataType>
void Time_Quad<FloatType,DataType>::checks()
{
	// size checks (not using error package)
	if (L_kernel %2 != 0){
		printf(" L_kernel is not even dont expect this to work... \n");
	}
	if (L_kernel > L_FFT){
		printf(" L_kernel > L_FFT dont expect this to work... \n");
	}
	if (L_kernel > L_data){
		printf(" L_kernel > L_data dont expect this to work... \n");
	}
}

template <class FloatType, class DataType>
void Time_Quad<FloatType,DataType>::cast_and_alloc()
{
	// Memory allocation and type declaration for Numpy_Array_Interface type
	np_data.recallocate(L_data) ; 
	np_kernel_p.recallocate(L_kernel) ; 
	np_kernel_q.recallocate(L_kernel) ; 
	np_p.recallocate(L_full) ; 
	np_q.recallocate(L_full) ;
	
	np_p_valid.set_BufferDimension( np_p.get_BufferDimension() - 2*L_kernel + 2 ) ;
	np_p_valid.set_BufferData ( np_p.get_BufferData() + L_kernel - 1 ) ;
	np_q_valid.set_BufferDimension( np_q.get_BufferDimension() - 2*L_kernel + 2 ) ;
	np_q_valid.set_BufferData ( np_q.get_BufferData() + L_kernel - 1 ) ;
	
	p_data = np_data.get_BufferData();
	p_kernel_p = np_kernel_p.get_BufferData();
	p_kernel_q = np_kernel_q.get_BufferData();
	p_p = np_p.get_BufferData();
	p_q = np_q.get_BufferData();
	
	#ifdef DEBUG 
		np_FFT_kernel_p.recallocate( L_FFT/2+1 ) ;
		np_FFT_kernel_q.recallocate( L_FFT/2+1 ) ;
		p_FFT_kernel_p = np_FFT_kernel_p.get_BufferData();
		p_FFT_kernel_q = np_FFT_kernel_q.get_BufferData();
	#endif
}

template <class FloatType, class DataType>
void Time_Quad<FloatType,DataType>::set_L_FFT( uint32_t L_FFT )
{
	this->L_FFT = L_FFT;
	this->L_chunk = L_FFT - this->L_kernel + 1; // Prevents cyclic convolution
	this->N_chunks = this->L_data/this->L_chunk;
	this->N_chunks = this->L_data/this->L_chunk;
	this->L_reste = this->L_data%this->L_chunk;
	
	checks();
};

template <class FloatType, class DataType>
void Time_Quad<FloatType,DataType>::make_kernels()const
{
	// Abscisse
	FloatType t[ L_kernel ] ;
	for (uint64_t i = 0; i < L_kernel/2; i++ ) // L_kernel doit être paire
	{
		uint64_t Milieu = L_kernel/2;
		FloatType tmp = ( i + 1 )*dt;
		t[ Milieu + i ] = tmp ; // Partie + 
		t[ Milieu -(i+1) ] = (-1)*tmp ; // Partie -
	}
	
	// Ordonnée
	for (uint64_t i = 0; i < L_kernel; i++ )
	{
		FloatType t_i = t[i];
		FloatType tmp = 1/( sqrt( abs( t_i ) ) );
		p_kernel_p[i] = tmp ;
		p_kernel_q[i] = copysign( tmp , t_i ) ;
	}
}
								
template <class FloatType, class DataType>
void Time_Quad<FloatType,DataType>::print_all_attributes()
{
	printf ("Input attributes : \n");
	printf ("\t L_kernel = %llu \n", L_kernel);
	printf ("\t L_data = %llu \n", L_data);
	printf ("\t L_FFT = %d \n" , L_FFT);
	printf ("\t dt = %f [ns]\n" , dt);
	std::cout << "\t Convolution_type = " << Convolution_type << "\n";
	printf ("Deduced attributes : \n");
	printf ("\t L_full = %llu \n", L_full);
	printf ("\t L_valid = %llu \n", L_valid);
	printf ("\t L_chunk = %llu \n", L_chunk);
	printf ("\t N_chunks = %llu \n", N_chunks);
	printf ("\t L_reste = %llu \n", L_reste);
	printf ("Important pointers \n");
	printf ("\t p_kernel_p = %p \n", p_kernel_p );
	printf ("\t p_kernel_q = %p \n", p_kernel_q );
	printf ("\t p_data = %p \n", p_data);
	printf ("\t p_p = %p \n", p_p);
	printf ("\t p_q = %p \n", p_q);
}

template <class FloatType, class DataType>
void Time_Quad<FloatType,DataType>::execute()
{
// h(f) = \int f(t) g(t-tau) dt

// Takes the convolution of f and g and output the result in h and crops with respect to the Output_length arguement
//	f and g have to be consistent with their size declared in the class attributes L_kernel and L_data

#ifdef DEBUG
	Timer Timer(&execute_time);
#endif

	if ( Convolution_type == "fft" )
	{	
		Conv_FFT( p_kernel_p , p_data , p_p );
		Conv_FFT( p_kernel_q , p_data , p_q );
	}
	else if ( Convolution_type == "direct" )
	{
		Conv_Directe( p_kernel_p , p_data , p_p );
		Conv_Directe( p_kernel_q , p_data , p_q );
	}
	else 
	{
		 printf(" Convolution_type attribute is invalid. \n");
	}
		
}

template <>
void Time_Quad<float,int16_t>::Conv_FFT_kernel(const float* Input, complex_f* Output )
{	
	// Tries to load wisdom if it exists
	fftwf_import_wisdom_from_filename("FFTWF_Wisdom.dat");
	
	fftwf_plan f_plan;
	
	// Prepare plan for real to half complex transform
	f_plan = fftwf_plan_dft_r2c_1d( L_FFT , (float*)Output , reinterpret_cast<fftwf_complex*>(Output) , FFTW_EXHAUSTIVE); // in-place fft
	// Value assignment and zero padding
	for(uint64_t i = 0 ; i < L_kernel ; i++){
		( (float*)Output )[i] = ( (float)Input[i] ) ; // Convert uint16_t to float
	}
	for(uint64_t i = L_kernel ; i < L_FFT ; i++){
		( (float*)Output )[i] = 0 ; //zero padding
	}
	fftwf_execute(f_plan); // f now contains the results of the fft
	
	// Saving wisdom for future use
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
	
	fftwf_destroy_plan(f_plan); 
	
}
template <>
void Time_Quad<double,int16_t>::Conv_FFT_kernel(const double* Input, complex_d* Output)
{
	//// Tries to load wisdom if it exists
	fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
	
	fftw_plan f_plan;
	
	////Prepare plan for real to half complex transform
	f_plan = fftw_plan_dft_r2c_1d( L_FFT , (double*)Output , reinterpret_cast<fftw_complex*>(Output), FFTW_EXHAUSTIVE); // in-place fft
	// Value assignment and zero padding
	for(uint64_t i = 0 ; i < L_kernel ; i++)
	{
		( (double*)Output )[i] = ( (double)Input[i] ) ; // Convert uint16_t to double
	}
	for(uint64_t i = L_kernel ; i < L_FFT ; i++)
	{
		( (double*)Output )[i] = 0 ; //zero padding
	}
	fftw_execute(f_plan); // f now contains the results of the fft
	
	////Saving wisdom for future use
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	
	fftw_destroy_plan(f_plan); 

}

template <>
void Time_Quad<float,int16_t>::Conv_FFT(const float* F ,const int16_t* G , float* H )
{
	// Tries to load wisdom if it exists
	fftwf_import_wisdom_from_filename("FFTWF_Wisdom.dat");
		
	// Kernel et fft kernel 
	complex_f* f_HalfC = (complex_f*)fftwf_malloc( 2*(L_FFT/2+1) * sizeof(float) ); // For memory alignment
	Conv_FFT_kernel( F , f_HalfC );
		
	// Output_type specific declarations
	fftwf_plan g_plan;
	fftwf_plan h_plan;
	
	// For in-place transforms g as 2( n/2 + 1 )  elements. The elements beyond the first n are unused padding.
	complex_f* g = (complex_f*)fftwf_malloc( 2 * (L_FFT/2+1) * sizeof(float) ); // fftwf_malloc for memory alignment 
	complex_f* h = (complex_f*)fftwf_malloc( 2 * (L_FFT/2+1) * sizeof(float) ); // fftwf_malloc for memory alignment 
	// For zero padding
	for(uint64_t j=0; j < (L_FFT/2+1) ; j++)
	{
		g[j] = 0;
	}
	
	// Prepare plans : r2c and c2r
	g_plan = fftwf_plan_dft_r2c_1d( L_FFT , (float*)g , reinterpret_cast<fftwf_complex*>(h) , FFTW_EXHAUSTIVE);
		//  in-place transform of h.
		//The c2r transform destroys its input array even for out-of-place transforms
	h_plan = fftwf_plan_dft_c2r_1d( L_FFT, reinterpret_cast<fftwf_complex*>(h) , (float*)h , FFTW_EXHAUSTIVE); 
	
	// loop on chunks and add the fft result to H
	///// The chunks ---->
	for(uint64_t i=0; i < N_chunks ; i++)
	//for(uint64_t i=0; i < 1 ; i++)
	{
		uint64_t j;
		// Copie-paste the right part of G into g
		for(j=0; j < L_chunk ; j++)
		{
			( (float*)g )[j] = ( (float)( G[i*L_chunk + j] ) );
		}
		// execute the transform of g 
		fftwf_execute(g_plan); // h now contains the results of the DFT of g
		
		//multiplication de f_HalfC et h
		for(j=0; j < (L_FFT/2+1) ; j++)
		{
			h[j] = f_HalfC[j] * h[j];
		}
		
		fftwf_execute(h_plan);  // g now contains the results of the chunk's contribution to the convolution product
		
		for(j=0; j < L_FFT ; j++)
		{
			H[i*L_chunk+j] += ( ( (float*)h )[j] )/L_FFT ;
		}
	}
	/////
	///// The rest ---->
		if (L_reste != 0)
		{	
			uint64_t j;
			// make sure g only contains zeros
			for(j=0; j < L_FFT ; j++)
			{
				( (float*)g )[j] = 0 ;
			}
			// add the rest
			for(j=0; j < L_reste ; j++)
			{
				( (float*)g )[j] = ( (float) G[N_chunks*L_chunk + j] );
			}
			// execute the transform of g 
			fftwf_execute(g_plan); // h now contains the results of the DFT of g
			// in-place multiplication de f_HalfC et h (aussi half complexe)
			for(j=0; j < (L_FFT/2+1) ; j++)
			{
				h[j] = f_HalfC[j] * h[j];
			}
			fftwf_execute(h_plan);  // g now contains the results of the chunk's contribution to the convolution product
		
			// Select only the part of the ifft that contributes to the full output length
			for(j = 0 ; j < L_reste + L_kernel - 1 ; j++ ){
				H[N_chunks*L_chunk+j] += ( ( (float*)h )[j] )/L_FFT;
			}
			// H now contains the full length convolution
		}
	/////
	// Saving wisdom for future use
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
	
	// free and cleanup
	fftwf_free(g); 
	fftwf_cleanup(); 
}

template <>
void Time_Quad<double,int16_t>::Conv_FFT(const double* F ,const int16_t* G , double* H )
{
	// Notice uppercase F, G and H : inputs list pointers
	// and lovercase f ,g and h: local representation for chunks
	
	// Tries to load wisdom if it exists
	fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
		
	// Kernel et fft kernel 
	complex_d* f_HalfC = (complex_d*)fftw_malloc( 2*(L_FFT/2+1) * sizeof(double) ); // For memory alignment
	Conv_FFT_kernel( F , f_HalfC );
		
	// Output_type specific declarations
	fftw_plan g_plan;
	fftw_plan h_plan;
	
	// For in-place transforms g as 2( n/2 + 1 )  elements. The elements beyond the first n are unused padding.
	complex_d* g = (complex_d*)fftw_malloc( 2 * (L_FFT/2+1) * sizeof(double) ); // fftw_malloc for memory alignment 
	complex_d* h = (complex_d*)fftw_malloc( 2 * (L_FFT/2+1) * sizeof(double) ); // fftw_malloc for memory alignment 
	// For zero padding
	for(uint64_t j=0; j < (L_FFT/2+1) ; j++)
	{
		g[j] = 0;
	}
	
	// Prepare plans : r2c and c2r
	g_plan = fftw_plan_dft_r2c_1d( L_FFT , (double*)g , reinterpret_cast<fftw_complex*>(h) , FFTW_EXHAUSTIVE);
		//  in-place transform of h.
		//The c2r transform destroys its input array even for out-of-place transforms
	h_plan = fftw_plan_dft_c2r_1d( L_FFT, reinterpret_cast<fftw_complex*>(h) , (double*)h , FFTW_EXHAUSTIVE); 
	
	// loop on chunks and add the fft result to H
	///// The chunks ---->
	for(uint64_t i=0; i < N_chunks ; i++)
	//for(uint64_t i=0; i < 1 ; i++)
	{
		uint64_t j;
		// Copie-paste the right part of G into g
		for(j=0; j < L_chunk ; j++)
		{
			( (double*)g )[j] = ( (double)( G[i*L_chunk + j] ) );
		}
		// execute the transform of g 
		fftw_execute(g_plan); // h now contains the results of the DFT of g
		
		//multiplication de f_HalfC et h
		for(j=0; j < (L_FFT/2+1) ; j++)
		{
			h[j] = f_HalfC[j] * h[j];
		}
		
		fftw_execute(h_plan);  // g now contains the results of the chunk's contribution to the convolution product
		
		for(j=0; j < L_FFT ; j++)
		{
			H[i*L_chunk+j] += ( ( (double*)h )[j] )/L_FFT ;
		}
	}
	/////
	///// The rest ---->
		if (L_reste != 0)
		{	
			uint64_t j;
			// make sure g only contains zeros
			for(j=0; j < L_FFT ; j++)
			{
				( (double*)g )[j] = 0 ;
			}
			// add the rest
			for(j=0; j < L_reste ; j++)
			{
				( (double*)g )[j] = ( (double) G[N_chunks*L_chunk + j] );
			}
			// execute the transform of g 
			fftw_execute(g_plan); // h now contains the results of the DFT of g
			// in-place multiplication de f_HalfC et h (aussi half complexe)
			for(j=0; j < (L_FFT/2+1) ; j++)
			{
				h[j] = f_HalfC[j] * h[j];
			}
			fftw_execute(h_plan);  // g now contains the results of the chunk's contribution to the convolution product
		
			// Select only the part of the ifft that contributes to the full output length
			for(j = 0 ; j < L_reste + L_kernel - 1 ; j++ ){
				H[N_chunks*L_chunk+j] += ( ( (double*)h )[j] )/L_FFT;
			}
			// H now contains the full length convolution
		}
	/////
	// Saving wisdom for future use
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	
	// free and cleanup
	fftw_free(g); 
	fftw_cleanup();
	
}



template <class FloatType, class DataType>
void Time_Quad<FloatType,DataType>::Conv_Directe( FloatType* f , DataType* g , FloatType* h)const
{
	// Begenning
	for(uint64_t i = 0 ; i < L_kernel - 1 ; i++)
	{
		for(uint64_t j = 0; j <= i  ; j++)
		{	
			h[i] += (FloatType)(f[j]* g[(i - j)]) ;
		}
	}
	// Middle (This is also the valid part)
	uint64_t translation =  L_kernel - 1;

	for(uint64_t i = 0 ; i < L_data - L_kernel + 1 ; i++)
	{
		for(uint64_t j=0; j < L_kernel ; j++)
		{
			h[i+translation] += g[ (L_kernel - 1) - j + i]*f[j] ;
		}
	}
	// end
	uint64_t end_full = L_full -1;
	
	for(uint64_t i=0; i < L_kernel - 1; i++)
	{
		for(uint64_t j=0; j <= i ; j++)
		{
			h[end_full-i] += f[(L_kernel-1)-j]*g[(L_data - 1)-i+j] ;
		}
	}
}

// debug tools
#ifdef DEBUG
	template<class FloatType, class DataType>
	void Time_Quad<FloatType,DataType>::execute_kernel()
	{
		Conv_FFT_kernel( p_kernel_p , p_FFT_kernel_p );
		Conv_FFT_kernel( p_kernel_q , p_FFT_kernel_q );
	}
#endif