//Constructor
template <class OutputType, class KernelType, class DataType, class ComplexType>
Mother_Fast_Conv<OutputType,KernelType,DataType,ComplexType>::Mother_Fast_Conv
(	
	uint64_t L_kernel,  uint64_t L_data, uint32_t L_FFT, std::string Convolution_type
):
	L_FFT(L_FFT) , Convolution_type(Convolution_type),
	L_kernel(L_kernel) , L_data(L_data),
	kernel(NULL), data(NULL), output(NULL)
{
	L_full = L_kernel + L_data - 1 ;
	L_valid = L_data - L_kernel + 1 ;
	
	set_L_FFT( L_FFT );
	
	// Checks for errors cast type and allocate memory for pointers and declare numpy arrays.
	checks();
	cast_and_alloc();
	
}

//Destructor
template <class OutputType, class KernelType, class DataType, class ComplexType>
Mother_Fast_Conv<OutputType,KernelType,DataType,ComplexType>::~Mother_Fast_Conv()
{	 
	kernel = NULL;
	output = NULL;
	data = NULL;
}

template <class OutputType, class KernelType, class DataType, class ComplexType>
void Mother_Fast_Conv<OutputType,KernelType,DataType,ComplexType>::checks()
{
	// size checks (not using error package)
	if (L_kernel %2 != 0)
	{
		throw std::runtime_error(" L_kernel is not even dont expect this to work... ");
	}
	if (L_kernel > L_FFT)
	{
		throw std::runtime_error(" L_kernel > L_FFT dont expect this to work... ");
	}
	if (L_kernel > L_data)
	{
		throw std::runtime_error(" L_kernel > L_data dont expect this to work... ");
	}
}

template <class OutputType, class KernelType, class DataType, class ComplexType>
void Mother_Fast_Conv<OutputType,KernelType,DataType,ComplexType>::cast_and_alloc()
{
	// Memory allocation and type declaration for Numpy_Array_Interface type
	np_data.recallocate(L_data) ; 
	np_kernel.recallocate(L_kernel) ; 
	np_output.recallocate(L_full) ; 
	np_output_valid.set_BufferDimension( L_valid ) ;
	np_output_valid.set_BufferData ( np_output.get_BufferData() + L_kernel - 1 ) ;
	
	data = np_data.get_BufferData();
	kernel = np_kernel.get_BufferData();
	output = np_output.get_BufferData();	
}

template <class OutputType, class KernelType, class DataType, class ComplexType>
void Mother_Fast_Conv<OutputType,KernelType,DataType,ComplexType>::set_L_FFT( uint32_t L_FFT )
{
	this->L_FFT = L_FFT;
	this->L_chunk = L_FFT - this->L_kernel + 1; // Prevents cyclic convolution
	this->N_chunks = this->L_data/this->L_chunk;
	this->N_chunks = this->L_data/this->L_chunk;
	this->L_reste = this->L_data%this->L_chunk;
	
	checks();
};
								
template <class OutputType, class KernelType, class DataType, class ComplexType>
void Mother_Fast_Conv<OutputType,KernelType,DataType,ComplexType>::print_all_attributes()
{
	printf ("Input attributes : \n");
	printf ("\t L_kernel = %llu \n", L_kernel);
	printf ("\t L_data = %llu \n", L_data);
	printf ("\t L_FFT = %d \n" , L_FFT);
	std::cout << "\t Convolution_type = " << Convolution_type << "\n";
	printf ("Deduced attributes : \n");
	printf ("\t L_full = %llu \n", L_full);
	printf ("\t L_valid = %llu \n", L_valid);
	printf ("\t L_chunk = %llu \n", L_chunk);
	printf ("\t N_chunks = %llu \n", N_chunks);
	printf ("\t L_reste = %llu \n", L_reste);
	printf ("Important pointers \n");
	printf ("\t kernel = %p \n", kernel );
	printf ("\t data = %p \n", data);
	printf ("\t output = %p \n", output);
}

template <class OutputType, class KernelType, class DataType, class ComplexType>
void Mother_Fast_Conv<OutputType,KernelType,DataType,ComplexType>::Conv_basic( KernelType* f , DataType* g , OutputType* h)const
{
	// Begenning
	for(uint64_t i = 0 ; i < L_kernel - 1 ; i++)
	{
		for(uint64_t j = 0; j <= i  ; j++)
		{	
			h[i] += (OutputType)f[j] * (OutputType)g[(i - j)]  ;
		}
	}
	// Middle (This is also the valid part)
	uint64_t translation =  L_kernel - 1;

	for(uint64_t i = 0 ; i < L_data - L_kernel + 1 ; i++)
	{
		for(uint64_t j=0; j < L_kernel ; j++)
		{
			h[i+translation] += (OutputType)g[ (L_kernel - 1) - j + i] * (OutputType)f[j] ;
		}
	}
	// end
	uint64_t end_full = L_full -1;
	
	for(uint64_t i=0; i < L_kernel - 1; i++)
	{
		for(uint64_t j=0; j <= i ; j++)
		{
			h[end_full-i] += (OutputType)f[(L_kernel-1)-j] * (OutputType)g[(L_data - 1)-i+j] ;
		}
	}
}


//////// Specializations

template <class OutputType, class KernelType, class DataType>
void Fast_Conv<OutputType,KernelType,DataType,complex_f>::execute()
{
	KernelType* kernel = this->kernel;
	DataType* data = this->data;
	OutputType* output = this->output;

	if ( this->Convolution_type == "fft" )
	{	
		Conv_FFT( kernel , data , output );
	}
	else if ( this->Convolution_type == "basic" )
	{
		this->Conv_basic( kernel , data , output );
	}
	else 
	{
		throw std::runtime_error("Convolution_type attribute is invalid.");
	}
		
}

template <class OutputType, class KernelType, class DataType>
void Fast_Conv<OutputType,KernelType,DataType,complex_d>::execute()
{
	KernelType* kernel = this->kernel;
	DataType* data = this->data;
	OutputType* output = this->output;

	if ( this->Convolution_type == "fft" )
	{	
		Conv_FFT( kernel , data , output );
	}
	else if ( this->Convolution_type == "basic" )
	{
		this->Conv_basic( kernel , data , output );
	}
	else 
	{
		throw std::runtime_error("Convolution_type attribute is invalid.");
	}
		
}

template <class OutputType, class KernelType, class DataType>
inline void Fast_Conv<OutputType,KernelType,DataType,complex_f>::FFT_kernel(const KernelType* In, complex_f* Out)
{	
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	
	// Tries to load wisdom if it exists
	fftwf_import_wisdom_from_filename("FFTWF_Wisdom.dat");
	
	fftwf_plan f_plan;
	
	// Prepare plan for real to half complex transform
	f_plan = fftwf_plan_dft_r2c_1d( L_FFT , (float*)Out , reinterpret_cast<fftwf_complex*>(Out) , FFTW_EXHAUSTIVE); // in-place fft
	// Value assignment and zero padding
	for(uint64_t i = 0 ; i < L_kernel ; i++){
		( (float*)Out )[i] = ( (float)In[i] ) ; // Convert IntType to float
	}
	for(uint64_t i = L_kernel ; i < L_FFT ; i++){
		( (float*)Out )[i] = 0 ; //zero padding
	}
	fftwf_execute(f_plan); // f now contains the results of the fft
	
	// Saving wisdom for future use
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
	
	fftwf_destroy_plan(f_plan); 	
}

template <class OutputType, class KernelType, class DataType>
inline void Fast_Conv<OutputType,KernelType,DataType,complex_d>::FFT_kernel(const KernelType* In, complex_d* Out)
{	
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	// Tries to load wisdom if it exists
	fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
	
	fftw_plan f_plan;
	
	// Prepare plan for real to half complex transform
	f_plan = fftw_plan_dft_r2c_1d( L_FFT , (double*)Out , reinterpret_cast<fftw_complex*>(Out) , FFTW_EXHAUSTIVE); // in-place fft
	// Value assignment and zero padding
	for(uint64_t i = 0 ; i < L_kernel ; i++){
		( (double*)Out )[i] = ( (double)In[i] ) ; // Convert IntType to float
	}
	for(uint64_t i = L_kernel ; i < L_FFT ; i++){
		( (double*)Out )[i] = 0 ; //zero padding
	}
	fftw_execute(f_plan); // f now contains the results of the fft
	
	// Saving wisdom for future use
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	
	fftw_destroy_plan(f_plan); 	
}

template <class OutputType, class KernelType, class DataType>
inline void Fast_Conv<OutputType,KernelType,DataType,complex_f>::Conv_FFT(const KernelType* Kernel ,const DataType* Data , OutputType* Out )
{
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	uint64_t N_chunks = this->N_chunks;
	uint64_t L_chunk = this->L_chunk;
	uint64_t L_reste = this->L_reste;
	
	// Tries to load wisdom if it exists
	fftwf_import_wisdom_from_filename("FFTWF_Wisdom.dat");
		
	// Kernel et fft kernel 
	complex_f* f_HalfC = (complex_f*)fftwf_malloc( 2*(L_FFT/2+1) * sizeof(float) ); // For memory alignment
	FFT_kernel( Kernel , f_HalfC );
		
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
	
	// loop on chunks and add the fft result to Out
	///// The chunks ---->
	for(uint64_t i=0; i < N_chunks ; i++)
	//for(uint64_t i=0; i < 1 ; i++)
	{
		uint64_t j;
		// Copie-paste the right part of Data into g
		for(j=0; j < L_chunk ; j++)
		{
			( (float*)g )[j] = ( (float)( Data[i*L_chunk + j] ) );
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
			Out[i*L_chunk+j] += (OutputType)( ( ( (float*)h )[j] )/L_FFT );
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
				( (float*)g )[j] = ( (float) Data[N_chunks*L_chunk + j] );
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
				Out[N_chunks*L_chunk+j] += (OutputType)( ( ( (float*)h )[j] )/L_FFT );
			}
			// Out now contains the full length convolution
		}
	/////
	// Saving wisdom for future use
	fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
	
	// free and cleanup
	fftwf_free(g); 
	fftwf_cleanup(); 
}

template <class OutputType, class KernelType, class DataType>
inline void Fast_Conv<OutputType,KernelType,DataType,complex_d>::Conv_FFT(const KernelType* Kernel ,const DataType* Data , OutputType* Out )
{
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	uint64_t N_chunks = this->N_chunks;
	uint64_t L_chunk = this->L_chunk;
	uint64_t L_reste = this->L_reste;
	
	// Tries to load wisdom if it exists
	fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
		
	// Kernel et fft kernel 
	complex_d* f_HalfC = (complex_d*)fftwf_malloc( 2*(L_FFT/2+1) * sizeof(double) ); // For memory alignment
	FFT_kernel( Kernel , f_HalfC );
		
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
	
	// loop on chunks and add the fft result to Out
	///// The chunks ---->
	for(uint64_t i=0; i < N_chunks ; i++)
	//for(uint64_t i=0; i < 1 ; i++)
	{
		uint64_t j;
		// Copie-paste the right part of Data into g
		for(j=0; j < L_chunk ; j++)
		{
			( (double*)g )[j] = ( (double)( Data[i*L_chunk + j] ) );
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
			Out[i*L_chunk+j] += (OutputType)( ( ( (double*)h )[j] )/L_FFT );
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
				( (double*)g )[j] = ( (double) Data[N_chunks*L_chunk + j] );
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
				Out[N_chunks*L_chunk+j] += (OutputType)( ( ( (double*)h )[j] )/L_FFT );
			}
			// Out now contains the full length convolution
		}
	/////
	// Saving wisdom for future use
	fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
	
	// free and cleanup
	fftw_free(g); 
	fftw_cleanup(); 
}


