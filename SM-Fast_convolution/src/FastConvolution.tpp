//MOTHER CONSTRUCTOR
template <class OutputType, class KernelType, class DataType, class ComplexType>
Mother_FastConvolution<OutputType,KernelType,DataType,ComplexType>::Mother_FastConvolution
(	
	uint64_t L_kernel,  uint64_t L_data, uint32_t L_FFT, std::string Convolution_type, int n_threads
):
	Convolution_type(Convolution_type)
    , Time_execution(0)
    ,L_FFT(L_FFT)
	,n_threads(n_threads)
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
	cast_and_alloc();
}

//MOTHER DESTRUCTOR
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// CHILD CLASS PARTIAL SPECIALIZATION

//CHILD CONSTRUCTORS
template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_f>::FastConvolution 
	(
	uint64_t L_kernel , uint64_t L_data, uint32_t L_FFT , std::string Convolution_type , int n_threads
	)
	: Mother_FastConvolution<OutputType,KernelType,DataType,complex_f>(L_kernel , L_data , L_FFT , Convolution_type, n_threads)
    , kernel_complex(NULL), gs(NULL), hs(NULL)
{	
    // printf( "float : Begin Consgtructor \n" );
    omp_set_num_threads(n_threads);
    this->n_threads = n_threads ;
    // printf("\t n_threads = %d \n", n_threads);
    fftwf_import_wisdom_from_filename("FFTWF_Wisdom.dat");
    prepare_heap();
    prepare_plans();
    // printf( "float : End Consgtructor \n" );
}

template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_d>::FastConvolution 
	(
	uint64_t L_kernel , uint64_t L_data, uint32_t L_FFT , std::string Convolution_type , int n_threads
	)
	: Mother_FastConvolution<OutputType,KernelType,DataType,complex_d>(L_kernel , L_data , L_FFT , Convolution_type, n_threads)
    , kernel_complex(NULL), gs(NULL), hs(NULL)
{	
    // printf( "double : Begin Consgtructor \n" );
    omp_set_num_threads(n_threads);
    this->n_threads = n_threads ;
    fftw_import_wisdom_from_filename("FFTW_Wisdom.dat");
    prepare_heap();
    prepare_plans();
    // printf( "double : End Consgtructor \n" );
}

//CHILD DESTRUCTOR
template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_f>::~FastConvolution()
{
    // printf( "float : Begin Destructor \n" );
    free_daughter();
    fftwf_cleanup(); 
    // printf( "float : End Destructor \n" );
}

template <class OutputType, class KernelType, class DataType>
FastConvolution<OutputType,KernelType,DataType,complex_d>::~FastConvolution()
{
    // printf( "double : Begin Destructor \n" );
    free_daughter();
    fftw_cleanup(); 
    // printf( "double : End Destructor \n" );
}

// SET_N_THREADS METHODS
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::set_n_threads( int n_threads )
{
    // printf( "float : Begin set_n_threads \n" );
    free_daughter();
    this->n_threads = n_threads ;
    omp_set_num_threads(n_threads);
    prepare_heap();
    prepare_plans();
    // printf( "float : End set_n_threads \n" );
}

template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_d>::set_n_threads( int n_threads )
{
    // printf( "double : Begin set_n_threads \n" );
    free_daughter();
    this->n_threads = n_threads ;
    omp_set_num_threads(n_threads);
    prepare_heap();
    prepare_plans();
    // printf( "double : End set_n_threads \n" );
}

// SET_L_FFT METHODS
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::set_L_FFT( uint32_t L_FFT )
{
    // printf( "float : Begin set_L_FFT \n" );
    free_daughter();
    this->L_FFT = L_FFT;
	this->L_chunk = L_FFT - this->L_kernel + 1; // Prevents cyclic convolution
	this->N_chunks = this->L_data/this->L_chunk;
	this->L_reste = this->L_data%this->L_chunk;
	this->checks();
    prepare_heap();
    prepare_plans();
    // printf( "float : End set_L_FFT \n" );
}

template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_d>::set_L_FFT( uint32_t L_FFT )
{
    // printf( "double : Begin set_L_FFT \n" );
    free_daughter();
    this->L_FFT = L_FFT;
	this->L_chunk = L_FFT - this->L_kernel + 1; // Prevents cyclic convolution
	this->N_chunks = this->L_data/this->L_chunk;
	this->L_reste = this->L_data%this->L_chunk;
	this->checks();
    prepare_heap();
    prepare_plans();
    // printf( "double : End set_L_FFT \n" );
}

// PREPARE_HEAP METHODS
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::prepare_heap()
{
    // printf( "float : Begin prepare_heap \n" );
    std::string Convolution_type = this->Convolution_type;
	uint32_t L_FFT = this->L_FFT;
    int n_threads = this->n_threads;
  
    if ( Convolution_type == "fft" )
	{	
        kernel_complex = (complex_f*)fftwf_malloc( 2*(L_FFT/2+1) * sizeof(float) ); // For memory alignment
        // For in-place transforms g as 2( n/2 + 1 )  elements. The elements beyond the first n are unused padding.
        gs = (complex_f **) malloc(n_threads* sizeof(float*));
        hs = (complex_f **) malloc(n_threads* sizeof(float*));  
        #pragma omp parallel
        {
            gs[omp_get_thread_num()] = (complex_f*)fftwf_malloc( 2 * (L_FFT/2+1) * sizeof(float) ); // fftwf_malloc for memory alignment;
            hs[omp_get_thread_num()] = (complex_f*)fftwf_malloc( 2 * (L_FFT/2+1) * sizeof(float) ); // fftwf_malloc for memory alignment;
        }
	}
    // printf( "float : End prepare_heap  \n" );
}

template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_d>::prepare_heap()
{
    // printf( "double : Begin prepare_heap \n" );
    std::string Convolution_type = this->Convolution_type;
	uint32_t L_FFT = this->L_FFT;
    int n_threads = this->n_threads;
  
    if ( Convolution_type == "fft" )
	{	
        kernel_complex = (complex_d*)fftw_malloc( 2*(L_FFT/2+1) * sizeof(double) ); // For memory alignment
        // For in-place transforms g as 2( n/2 + 1 )  elements. The elements beyond the first n are unused padding.
        gs = (complex_d **) malloc(n_threads* sizeof(double*));
        hs = (complex_d **) malloc(n_threads* sizeof(double*));  
        #pragma omp parallel
        {
            gs[omp_get_thread_num()] = (complex_d*)fftw_malloc( 2 * (L_FFT/2+1) * sizeof(double) ); // fftw_malloc for memory alignment;
            hs[omp_get_thread_num()] = (complex_d*)fftw_malloc( 2 * (L_FFT/2+1) * sizeof(double) ); // fftw_malloc for memory alignment;
        }
	}
    // printf( "double : End prepare_heap  \n" );
}

// PREPARE_PLANS METHODS
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::prepare_plans()
{   
    // printf( "flaot : Begin prepare_plans \n" );
    std::string Convolution_type = this->Convolution_type;
	uint32_t L_FFT = this->L_FFT;
    
    if ( Convolution_type == "fft" )
	{	
        // Prepare plan for real to half complex transform
        kernel_plan = fftwf_plan_dft_r2c_1d( L_FFT , (float*)kernel_complex , reinterpret_cast<fftwf_complex*>(kernel_complex) , FFTW_EXHAUSTIVE); // in-place fft
        // Prepare plans : r2c and c2r
        g_plan = fftwf_plan_dft_r2c_1d( L_FFT , (float*)gs[0] , reinterpret_cast<fftwf_complex*>(hs[0]) , FFTW_EXHAUSTIVE);
            //  in-place transform of h.
            //The c2r transform destroys its input array even for out-of-place transforms
        h_plan = fftwf_plan_dft_c2r_1d( L_FFT, reinterpret_cast<fftwf_complex*>(hs[0]) , (float*)hs[0] , FFTW_EXHAUSTIVE); 
	}
    // printf( "float : End prepare_plans \n" );
}

template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_d>::prepare_plans()
{   
    // printf( "double : Begin prepare_plans \n" );
    std::string Convolution_type = this->Convolution_type;
	uint32_t L_FFT = this->L_FFT;
    
    if ( Convolution_type == "fft" )
	{	
        // Prepare plan for real to half complex transform
        kernel_plan = fftw_plan_dft_r2c_1d( L_FFT , (double*)kernel_complex , reinterpret_cast<fftw_complex*>(kernel_complex) , FFTW_EXHAUSTIVE); // in-place fft
        // Prepare plans : r2c and c2r
        g_plan = fftw_plan_dft_r2c_1d( L_FFT , (double*)gs[0] , reinterpret_cast<fftw_complex*>(hs[0]) , FFTW_EXHAUSTIVE);
            //  in-place transform of h.
            //The c2r transform destroys its input array even for out-of-place transforms
        h_plan = fftw_plan_dft_c2r_1d( L_FFT, reinterpret_cast<fftw_complex*>(hs[0]) , (double*)hs[0] , FFTW_EXHAUSTIVE); 
	}
    // printf( "double : End prepare_plans \n" );
}

// FRREE_DAUGHTER METHODS
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::free_daughter()
{
    // printf( "float : Begin free_daughter \n" );
    int n_threads = this->n_threads;
    fftwf_export_wisdom_to_filename("FFTWF_Wisdom.dat");
    // fftwf_destroy_plan(kernel_plan); 
    // fftwf_destroy_plan(g_plan); 
    // fftwf_destroy_plan(h_plan);
    
    // printf( "float : \t Before free kernel \n" );
    fftwf_free(kernel_complex); kernel_complex = NULL ;
    // printf( "float : \t Before free gs hs \n" );
    for(int i = 0 ; i < n_threads ; i++)
    {
        fftwf_free( gs[i] ) ; gs[i] = NULL ;
        fftwf_free( hs[i] ) ; hs[i] = NULL ;
    }
    free(gs) ; gs = NULL ;
    free(hs) ; hs = NULL ;
    // printf( "float : End free_daughter \n" );
}

template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_d>::free_daughter()
{
    int n_threads = this->n_threads;
    // printf( "double : Begin free_daughter \n" );
    fftw_export_wisdom_to_filename("FFTW_Wisdom.dat");
    // printf( "double : Before export wisdom \n" );
    // fftw_destroy_plan(kernel_plan); 
    // fftw_destroy_plan(g_plan); 
    // fftw_destroy_plan(h_plan);
    
    fftw_free(kernel_complex); kernel_complex = NULL ;
    for(int i = 0 ; i < n_threads ; i++)
    {
        fftw_free( gs[i] ) ; gs[i] = NULL ;
        fftw_free( hs[i] ) ; hs[i] = NULL ;
    }
    free(gs) ; gs = NULL ;
    free(hs) ; hs = NULL ;
    // printf( "double : End free_daughter \n" );
}

// EXECUTIONS METHODS
template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_f>::execute()
{   
    // printf( "Begin exxecute \n" );
	Timer Timer(& this->Time_execution); 
	KernelType* kernel = this->kernel;
	DataType* data = this->data;
	OutputType* output = this->output;
    std::string Convolution_type = this->Convolution_type;
	
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	uint64_t N_chunks = this->N_chunks;
	uint64_t L_chunk = this->L_chunk;
	uint64_t L_reste = this->L_reste;
    // int n_threads = this->n_threads;

	if ( Convolution_type == "fft" )
	{	
		///////////////////////////////////////////////
		//// KERNEL FFT
		// Value assignment and zero padding
		for(uint64_t i = 0 ; i < L_kernel ; i++)
        {
			( (float*)kernel_complex )[i] = (float)kernel[i] ; // Convert IntType to float
		}
		for(uint64_t i = L_kernel ; i < L_FFT ; i++)
        {
			( (float*)kernel_complex )[i] = 0 ; //zero padding
		}
        
		fftwf_execute(kernel_plan); // f now contains the results of the fft
		///////////////////////////////////////////////
		
        #pragma omp parallel
        {
            manage_thread_affinity();
            
            int this_thread = omp_get_thread_num();
            
            // For zero padding
            for(uint64_t j=0; j < (L_FFT/2+1) ; j++)
            {
                gs[this_thread][j] = 0;
            }
		
        
		// loop on chunks and add the fft result to Out
		///// The chunks ---->
            #pragma omp for
            for(uint64_t i=0; i < N_chunks ; i++)
            {
                uint64_t j;
                // Copie-paste the right part of Data into g
                for(j=0; j < L_chunk ; j++)
                {
                    ( (float*)gs[this_thread])[j] = ( (float)data[i*L_chunk + j] );
                }
                // execute the transform of g 
                fftwf_execute_dft_r2c(g_plan, (float*)gs[this_thread] , reinterpret_cast<fftwf_complex*>(hs[this_thread]) ); // h now contains the results of the DFT of g
                
                //multiplication de kernel_complex et h
                for(j=0; j < (L_FFT/2+1) ; j++)
                {
                    hs[this_thread][j] = kernel_complex[j] * hs[this_thread][j];
                }
                
                fftwf_execute_dft_c2r(h_plan , reinterpret_cast<fftwf_complex*>(hs[this_thread]) , (float*)hs[this_thread] );  // h now contains the results of the chunk's contribution to the convolution product
                
                // This is not thread safe...
                for(j=0; j < L_chunk ; j++)
                {
                    output[i*L_chunk+j] += (OutputType)( ( ( (float*)hs[this_thread])[j] )/L_FFT );
                }
                for(j=L_chunk; j < L_FFT ; j++)
                {
                    #pragma omp atomic update
                    output[i*L_chunk+j] += (OutputType)( ( ( (float*)hs[this_thread])[j] )/L_FFT );
                }
                //////////////////
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
					( (float*)gs[0] )[j] = 0 ;
				}
				// add the rest
				for(j=0; j < L_reste ; j++)
				{
					( (float*)gs[0] )[j] = ( (float) data[N_chunks*L_chunk + j] );
				}
				// execute the transform of g 
				fftwf_execute_dft_r2c(g_plan, (float*)gs[0] , reinterpret_cast<fftwf_complex*>(hs[0]) ); // h now contains the results of the DFT of g
				// in-place multiplication de kernel_complex et h (aussi half complexe)
				for(j=0; j < (L_FFT/2+1) ; j++)
				{
					hs[0][j] = kernel_complex[j] * hs[0][j];
				}
				fftwf_execute_dft_c2r(h_plan , reinterpret_cast<fftwf_complex*>(hs[0]) , (float*)hs[0] );  // h now contains the results of the chunk's contribution to the convolution product
			
				// Select only the part of the ifft that contributes to the full output length
				for(j = 0 ; j < L_reste + L_kernel - 1 ; j++ ){
					output[N_chunks*L_chunk+j] += (OutputType)( ( ( (float*)hs[0] )[j] )/L_FFT );
				}
				// Out now contains the full length convolution
			}
		///// 
	}
	else if ( Convolution_type == "basic" )
	{
		this->Conv_basic( kernel , data , output );
	}
	else 
	{
		throw std::runtime_error("Convolution_type attribute is invalid.");
	}
	// printf( "End exxecute \n" );	
}

template <class OutputType, class KernelType, class DataType>
void FastConvolution<OutputType,KernelType,DataType,complex_d>::execute()
{   
    // printf( "Begin exxecute \n" );
	Timer Timer(& this->Time_execution); 
	KernelType* kernel = this->kernel;
	DataType* data = this->data;
	OutputType* output = this->output;
    std::string Convolution_type = this->Convolution_type;
	
	uint64_t L_kernel = this->L_kernel;
	uint32_t L_FFT = this->L_FFT;
	uint64_t N_chunks = this->N_chunks;
	uint64_t L_chunk = this->L_chunk;
	uint64_t L_reste = this->L_reste;
    // int n_threads = this->n_threads;

	if ( Convolution_type == "fft" )
	{	
		///////////////////////////////////////////////
		//// KERNEL FFT
		// Value assignment and zero padding
		for(uint64_t i = 0 ; i < L_kernel ; i++)
        {
			( (double*)kernel_complex )[i] = (double)kernel[i] ; // Convert IntType to double
		}
		for(uint64_t i = L_kernel ; i < L_FFT ; i++)
        {
			( (double*)kernel_complex )[i] = 0 ; //zero padding
		}
        
		fftw_execute(kernel_plan); // f now contains the results of the fft
		///////////////////////////////////////////////
		
        #pragma omp parallel
        {
            manage_thread_affinity();
            
            int this_thread = omp_get_thread_num();
            
            // For zero padding
            for(uint64_t j=0; j < (L_FFT/2+1) ; j++)
            {
                gs[this_thread][j] = 0;
            }
		
        
		// loop on chunks and add the fft result to Out
		///// The chunks ---->
            #pragma omp for
            for(uint64_t i=0; i < N_chunks ; i++)
            {
                uint64_t j;
                // Copie-paste the right part of Data into g
                for(j=0; j < L_chunk ; j++)
                {
                    ( (double*)gs[this_thread])[j] = ( (double)data[i*L_chunk + j] );
                }
                // execute the transform of g 
                fftw_execute_dft_r2c(g_plan, (double*)gs[this_thread] , reinterpret_cast<fftw_complex*>(hs[this_thread]) ); // h now contains the results of the DFT of g
                
                //multiplication de kernel_complex et h
                for(j=0; j < (L_FFT/2+1) ; j++)
                {
                    hs[this_thread][j] = kernel_complex[j] * hs[this_thread][j];
                }
                
                fftw_execute_dft_c2r(h_plan , reinterpret_cast<fftw_complex*>(hs[this_thread]) , (double*)hs[this_thread] );  // h now contains the results of the chunk's contribution to the convolution product
                
                for(j=0; j < L_chunk ; j++)
                {
                    output[i*L_chunk+j] += (OutputType)( ( ( (double*)hs[this_thread])[j] )/L_FFT );
                }
                for(j=L_chunk; j < L_FFT ; j++)
                {
                    #pragma omp atomic update
                    output[i*L_chunk+j] += (OutputType)( ( ( (double*)hs[this_thread])[j] )/L_FFT );
                }
                //////////////////
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
					( (double*)gs[0] )[j] = 0 ;
				}
				// add the rest
				for(j=0; j < L_reste ; j++)
				{
					( (double*)gs[0] )[j] = ( (double) data[N_chunks*L_chunk + j] );
				}
				// execute the transform of g 
				fftw_execute(g_plan); // h now contains the results of the DFT of g
				// in-place multiplication de kernel_complex et h (aussi half complexe)
				for(j=0; j < (L_FFT/2+1) ; j++)
				{
					hs[0][j] = kernel_complex[j] * hs[0][j];
				}
				fftw_execute(h_plan);  // g now contains the results of the chunk's contribution to the convolution product
			
				// Select only the part of the ifft that contributes to the full output length
				for(j = 0 ; j < L_reste + L_kernel - 1 ; j++ ){
					output[N_chunks*L_chunk+j] += (OutputType)( ( ( (double*)hs[0] )[j] )/L_FFT );
				}
				// Out now contains the full length convolution
			}
		///// 
	}
	else if ( Convolution_type == "basic" )
	{
		this->Conv_basic( kernel , data , output );
	}
	else 
	{
		throw std::runtime_error("Convolution_type attribute is invalid.");
	}
	// printf( "End exxecute \n" );	
}
