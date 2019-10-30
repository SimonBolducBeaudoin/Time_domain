//Constructor
template <class OutputType, class KernelType, class DataType, class ComplexType>
Mother_FastConvolution<OutputType,KernelType,DataType,ComplexType>::Mother_FastConvolution
(	
	uint64_t L_kernel,  uint64_t L_data, uint32_t L_FFT, std::string Convolution_type, int n_threads
):
	L_FFT(L_FFT) , Convolution_type(Convolution_type)
	,n_threads(n_threads)
    // ,Time_alloc(0)  , Time_preparation(0), Time_execution(0)
	,L_kernel(L_kernel) , L_data(L_data)
	,kernel(NULL), data(NULL), output(NULL)
{
	L_full = L_kernel + L_data - 1 ;
	L_valid = L_data - L_kernel + 1 ;
	
	L_chunk = L_FFT - L_kernel + 1; // Prevents cyclic convolution
	N_chunks = L_data/L_chunk;
	N_chunks = L_data/L_chunk;
	L_reste = L_data%L_chunk;
	
	// Checks for errors cast type and allocate memory for pointers and declare numpy arrays.
	checks();
    omp_set_num_threads(n_threads);
	cast_and_alloc();
}

//Destructor
template <class OutputType, class KernelType, class DataType, class ComplexType>
Mother_FastConvolution<OutputType,KernelType,DataType,ComplexType>::~Mother_FastConvolution()
{	
	kernel = NULL;
	output = NULL;
	data = NULL;
}

template <class OutputType, class KernelType, class DataType, class ComplexType>
void Mother_FastConvolution<OutputType,KernelType,DataType,ComplexType>::checks()
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
void Mother_FastConvolution<OutputType,KernelType,DataType,ComplexType>::cast_and_alloc()
{
	// Memory allocation and type declaration for Numpy_Array_Interface type
    { 
        // Timer Timer(& this->Time_alloc);
        np_data.recallocate(L_data) ; 
        np_kernel.recallocate(L_kernel) ; 
        np_output.recallocate(L_full) ; 
    }
    // Valid is turned util Burred_array is replaced by a better solution
	// np_output_valid.set_BufferDimension( L_valid ) ;
	// np_output_valid.set_BufferData ( np_output.get_BufferData() + L_kernel - 1 ) ;
	
	data = np_data.get_BufferData();
	kernel = np_kernel.get_BufferData();
	output = np_output.get_BufferData();	
}

template <class OutputType, class KernelType, class DataType, class ComplexType>
void Mother_FastConvolution<OutputType,KernelType,DataType,ComplexType>::Conv_basic( KernelType* f , DataType* g , OutputType* h)const
{
	// Begenning
	for(uint64_t i = 0 ; i < L_kernel - 1 ; i++)
	{
		for(uint64_t j = 0; j <= i  ; j++)
		{	
			h[i] += (OutputType) ( f[j] * g[(i - j)] ) ;
		}
	}
	// Middle (This is also the valid part)
	uint64_t translation =  L_kernel - 1;

	for(uint64_t i = 0 ; i < L_data - L_kernel + 1 ; i++)
	{
		for(uint64_t j=0; j < L_kernel ; j++)
		{
			h[i+translation] += (OutputType)(g[ (L_kernel - 1) - j + i] * f[j] ) ;
		}
	}
	// end
	uint64_t end_full = L_full -1;
	
	for(uint64_t i=0; i < L_kernel - 1; i++)
	{
		for(uint64_t j=0; j <= i ; j++)
		{
			h[end_full-i] += (OutputType)(f[(L_kernel-1)-j]  * g[(L_data - 1)-i+j] ) ;
		}
	}
}

////////////////////////////////////////////
//// Child class partial specialization

// Constructors
template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_f>::FastConvolution 
	(
	uint64_t L_kernel , uint64_t L_data, uint32_t L_FFT , std::string Convolution_type , int n_threads
	)
	: Mother_FastConvolution<OutputType,KernelType,DataType,complex_f>(L_kernel , L_data , L_FFT , Convolution_type, n_threads)
    , kernel_complex(NULL), g(NULL), h(NULL) 
{	
	fftwf_import_wisdom_from_filename("FFTWF_Parallel_Wisdom.dat"); 
}

template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_d>::FastConvolution 
	(
	uint64_t L_kernel , uint64_t L_data, uint32_t L_FFT , std::string Convolution_type , int n_threads
	)
	: Mother_FastConvolution<OutputType,KernelType,DataType,complex_d>(L_kernel , L_data , L_FFT , Convolution_type, n_threads)
    , kernel_complex(NULL), g(NULL), h(NULL) 
{	
    fftw_import_wisdom_from_filename("FFTW_Parallel_Wisdom.dat");
}

// Destructors
template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_f>::~FastConvolution()
{
    fftwf_export_wisdom_to_filename("FFTWF_Parallel_Wisdom.dat");
	
    fftwf_destroy_plan(kernel_plan); 
    fftwf_destroy_plan(g_plan); 
    fftwf_destroy_plan(h_plan); 

    fftwf_free(kernel_complex); 
    fftwf_free(g); 
    fftwf_free(h); 
    
    fftwf_cleanup(); 
    fftwf_cleanup_threads();
}

template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_d>::~FastConvolution()
{
    // fftw_export_wisdom_to_filename("FFTW_Parallel_Wisdom.dat");
    // fftw_destroy_plan(kernel_plan); 
    // fftw_destroy_plan(g_plan); 
    // fftw_destroy_plan(h_plan); 

    // fftw_free(kernel_complex); 
    // fftw_free(g); 
    // fftw_free(h); 

    // fftw_cleanup(); 
    // fftw_cleanup_threads();
}

// set_n_threads methods
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::set_L_FFT( uint32_t L_FFT )
{
    fftwf_free(kernel_complex); kernel_complex = NULL ;
    fftwf_free(g); g = NULL ;
    fftwf_free(h); h = NULL ;
    
    this->L_FFT = L_FFT;
	this->L_chunk = L_FFT - this->L_kernel + 1; // Prevents cyclic convolution
	this->N_chunks = this->L_data/this->L_chunk;
	this->N_chunks = this->L_data/this->L_chunk;
	this->L_reste = this->L_data%this->L_chunk;
	
	this->checks();
}

template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_d>::set_L_FFT( uint32_t L_FFT )
{
    fftw_free(kernel_complex); kernel_complex = NULL ;
    fftw_free(g); g = NULL ;
    fftw_free(h); h = NULL ;
    
    this->L_FFT = L_FFT;
	this->L_chunk = L_FFT - this->L_kernel + 1; // Prevents cyclic convolution
	this->N_chunks = this->L_data/this->L_chunk;
	this->N_chunks = this->L_data/this->L_chunk;
	this->L_reste = this->L_data%this->L_chunk;
	
	this->checks();
}

// PREPARES METHODS

// template <class OutputType, class KernelType, class DataType>
// void FastConvolution<OutputType,KernelType,DataType,complex_f>::prepare()
// {
	// uint32_t L_FFT = this->L_FFT;
	// int n_threads = this->n_threads;

	// if ( this->Convolution_type == "fft" )
	// {	  

        // {
            // Timer Timer(& this->Time_alloc); 
            
            // kernel_complex = (complex_f*)fftwf_malloc( 2*(L_FFT/2+1) * sizeof(float) ); // For memory alignment
            ////For in-place transforms g as 2( n/2 + 1 )  elements. The elements beyond the first n are unused padding.
            // g = (complex_f*)fftwf_malloc( 2 * (L_FFT/2+1) * sizeof(float) ); // fftwf_malloc for memory alignment 
            // h = (complex_f*)fftwf_malloc( 2 * (L_FFT/2+1) * sizeof(float) ); // fftwf_malloc for memory alignment 
        // }
    
        // {
            // Timer Timer(& this->Time_preparation); 
            ////Must be declared before any plan
            // fftwf_plan_with_nthreads( n_threads );
            ////Prepare plan for real to half complex transform
            // kernel_plan = fftwf_plan_dft_r2c_1d( L_FFT , (float*)kernel_complex , reinterpret_cast<fftwf_complex*>(kernel_complex) , FFTW_PATIENT); // in-place fft
            ////Prepare plans : r2c and c2r
            // g_plan = fftwf_plan_dft_r2c_1d( L_FFT , (float*)g , reinterpret_cast<fftwf_complex*>(h) , FFTW_PATIENT);
                //// in-place transform of h.
                //// The c2r transform destroys its input array even for out-of-place transforms
            // h_plan = fftwf_plan_dft_c2r_1d( L_FFT, reinterpret_cast<fftwf_complex*>(h) , (float*)h , FFTW_PATIENT); 
        // }
	// }
	// else if ( this->Convolution_type == "basic" )
	// {
	 // /*PLACE HOLDER*/
	// }
	// else 
	// {
		// throw std::runtime_error("Convolution_type attribute is invalid.");
	// }
// }

// template <class OutputType, class KernelType, class DataType>
// void FastConvolution<OutputType,KernelType,DataType,complex_d>::prepare()
// {
	
	// uint32_t L_FFT = this->L_FFT;
	// int n_threads = this->n_threads;

	// if ( this->Convolution_type == "fft" )
	// {
         
		// {
            // Timer Timer(& this->Time_alloc); 
            
            // kernel_complex = (complex_d*)fftw_malloc( 2*(L_FFT/2+1) * sizeof(double) ); // For memory alignment
            ////For in-place transforms g as 2( n/2 + 1 )  elements. The elements beyond the first n are unused padding.
            // g = (complex_d*)fftw_malloc( 2 * (L_FFT/2+1) * sizeof(double) ); // fftw_malloc for memory alignment 
            // h = (complex_d*)fftw_malloc( 2 * (L_FFT/2+1) * sizeof(double) ); // fftw_malloc for memory alignment 
        // }
    
        // {
            // Timer Timer(& this->Time_preparation); 
            ////Must be declared before any plan
            // fftw_plan_with_nthreads( n_threads );
            ////Prepare plan for real to half complex transform
            // kernel_plan = fftw_plan_dft_r2c_1d( L_FFT , (double*)kernel_complex , reinterpret_cast<fftw_complex*>(kernel_complex) , FFTW_PATIENT); // in-place fft
            ////Prepare plans : r2c and c2r
            // g_plan = fftw_plan_dft_r2c_1d( L_FFT , (double*)g , reinterpret_cast<fftw_complex*>(h) , FFTW_PATIENT);
                //// in-place transform of h.
                ////The c2r transform destroys its input array even for out-of-place transforms
            // h_plan = fftw_plan_dft_c2r_1d( L_FFT, reinterpret_cast<fftw_complex*>(h) , (double*)h , FFTW_PATIENT); 
        // }
	// }
	// else if ( this->Convolution_type == "basic" )
	// {
	 // /*PLACE HOLDER*/
	// }
	// else 
	// {
		// throw std::runtime_error("Convolution_type attribute is invalid.");
	// }
// }

// EXCUTIONS METHODS
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::execute()
{
    // Timer Timer(& this->Time_execution); 
	KernelType* kernel = this->kernel;
	DataType* data = this->data;
	OutputType* output = this->output;
	
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	uint64_t N_chunks = this->N_chunks;
	uint64_t L_chunk = this->L_chunk;
	uint64_t L_reste = this->L_reste;

	if ( this->Convolution_type == "fft" )
	{	
		///////////////////////////////////////////////
		//// KERNEL FFT
		// Value assignment and zero padding
		for(uint64_t i = 0 ; i < L_kernel ; i++){
			( (float*)kernel_complex )[i] = (float)kernel[i] ; // Convert IntType to float
		}
		for(uint64_t i = L_kernel ; i < L_FFT ; i++){
			( (float*)kernel_complex )[i] = 0 ; //zero padding
		}
		fftwf_execute(kernel_plan); // f now contains the results of the fft
		///////////////////////////////////////////////
		
		// For zero padding
		for(uint64_t j=0; j < (L_FFT/2+1) ; j++)
		{
			g[j] = 0;
		}
		
		// loop on chunks and add the fft result to Out
		///// The chunks ---->
		for(uint64_t i=0; i < N_chunks ; i++)
		//for(uint64_t i=0; i < 1 ; i++)
		{
			uint64_t j;
			// Copie-paste the right part of Data into g
			for(j=0; j < L_chunk ; j++)
			{
				( (float*)g )[j] = ( (float)data[i*L_chunk + j] );
			}
			// execute the transform of g 
			fftwf_execute(g_plan); // g now contains the results of the DFT of g
			
			//multiplication de kernel_complex et h
			for(j=0; j < (L_FFT/2+1) ; j++)
			{
				h[j] = kernel_complex[j] * h[j];
			}
			
			fftwf_execute(h_plan);  // h now contains the results of the chunk's contribution to the convolution product
			
			for(j=0; j < L_FFT ; j++)
			{
				output[i*L_chunk+j] += (OutputType)( ( ( (float*)h )[j] )/L_FFT );
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
					( (float*)g )[j] = ( (float) data[N_chunks*L_chunk + j] );
				}
				// execute the transform of g 
				fftwf_execute(g_plan); // h now contains the results of the DFT of g
				// in-place multiplication de kernel_complex et h (aussi half complexe)
				for(j=0; j < (L_FFT/2+1) ; j++)
				{
					h[j] = kernel_complex[j] * h[j];
				}
				fftwf_execute(h_plan);  // g now contains the results of the chunk's contribution to the convolution product
			
				// Select only the part of the ifft that contributes to the full output length
				for(j = 0 ; j < L_reste + L_kernel - 1 ; j++ ){
					output[N_chunks*L_chunk+j] += (OutputType)( ( ( (float*)h )[j] )/L_FFT );
				}
				// Out now contains the full length convolution
			}
		/////
		
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
void FastConvolution<OutputType,KernelType,DataType,complex_d>::execute()
{
	KernelType* kernel = this->kernel;
	DataType* data = this->data;
	OutputType* output = this->output;
	
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	uint64_t N_chunks = this->N_chunks;
	uint64_t L_chunk = this->L_chunk;
	uint64_t L_reste = this->L_reste;

	if ( this->Convolution_type == "fft" )
	{	
		///////////////////////////////////////////////
		//// KERNEL FFT
		// Value assignment and zero padding
		for(uint64_t i = 0 ; i < L_kernel ; i++){
			( (double*)kernel_complex )[i] = (double)kernel[i] ; // Convert IntType to double
		}
		for(uint64_t i = L_kernel ; i < L_FFT ; i++){
			( (double*)kernel_complex )[i] = 0 ; //zero padding
		}
		fftw_execute(kernel_plan); // f now contains the results of the fft
		///////////////////////////////////////////////
		
		// For zero padding
		for(uint64_t j=0; j < (L_FFT/2+1) ; j++)
		{
			g[j] = 0;
		}
		
		// loop on chunks and add the fft result to Out
		///// The chunks ---->
		for(uint64_t i=0; i < N_chunks ; i++)
		//for(uint64_t i=0; i < 1 ; i++)
		{
			uint64_t j;
			// Copie-paste the right part of Data into g
			for(j=0; j < L_chunk ; j++)
			{
				( (double*)g )[j] = ( (double)data[i*L_chunk + j] );
			}
			// execute the transform of g 
			fftw_execute(g_plan); // g now contains the results of the DFT of g
			
			//multiplication de kernel_complex et h
			for(j=0; j < (L_FFT/2+1) ; j++)
			{
				h[j] = kernel_complex[j] * h[j];
			}
			
			fftw_execute(h_plan);  // h now contains the results of the chunk's contribution to the convolution product
			
			for(j=0; j < L_FFT ; j++)
			{
				output[i*L_chunk+j] += (OutputType)( ( ( (double*)h )[j] )/L_FFT );
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
					( (double*)g )[j] = ( (double) data[N_chunks*L_chunk + j] );
				}
				// execute the transform of g 
				fftw_execute(g_plan); // h now contains the results of the DFT of g
				// in-place multiplication de kernel_complex et h (aussi half complexe)
				for(j=0; j < (L_FFT/2+1) ; j++)
				{
					h[j] = kernel_complex[j] * h[j];
				}
				fftw_execute(h_plan);  // g now contains the results of the chunk's contribution to the convolution product
			
				// Select only the part of the ifft that contributes to the full output length
				for(j = 0 ; j < L_reste + L_kernel - 1 ; j++ ){
					output[N_chunks*L_chunk+j] += (OutputType)( ( ( (double*)h )[j] )/L_FFT );
				}
				// Out now contains the full length convolution
			}
		/////
		
	}
	else if ( this->Convolution_type == "basic" )
	{
		this->Conv_basic( kernel , data , output );
	}
	else 
	{
		throw std::runtime_error("Convolution_type attribute is invalid.");
	}
	
	fftw_export_wisdom_to_filename("FFTW_Parallel_Wisdom.dat");
    fftw_destroy_plan(kernel_plan); 
    fftw_destroy_plan(g_plan); 
    fftw_destroy_plan(h_plan); 

    fftw_free(kernel_complex); 
    fftw_free(g); 
    fftw_free(h); 

    fftw_cleanup(); 
    fftw_cleanup_threads();
		
}
