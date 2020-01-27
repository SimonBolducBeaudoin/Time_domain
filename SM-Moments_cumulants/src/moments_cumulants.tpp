/*
# Calculating moments, centered moments and cumulants from a univariate (i.e. 1D) histogram
*/


/*
$$ m_k = \sum_i p_i x_i^n $$
*/

template <class BinType>
double moment_k(BinType* histogram, uint64_t nofbins, uint k, double bin_centre, double bin_width)
{
    long double bin_value = 0;
    long double val = 0;
    uint64_t n=0;
    
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        #pragma omp for reduction(+:val), reduction(+:n)
        for (uint64_t i=0; i<nofbins; i++)
        {
            bin_value = (long double)(bin_width*i) + (long double)bin_centre ;
            val += (long double)histogram[i] * powl(bin_value, k);
            n += histogram[i];
        }
    }
    
    return (double)(val/(long double)n);
}


/*
$$ mu_k = \sum_i p_i (x_i-m_1)^n $$
*/
template <class BinType>
double centered_moment_k(BinType* histogram, uint64_t nofbins, uint k, double bin_centre, double bin_width)
{
    long double bin_value = 0;
    long double mean=0;
    long double val = 0;
    uint64_t n=0;
    
    if(k == 1)
    {
        return 0;
    }
    
    mean = moment_k(histogram, nofbins, 1, bin_centre, bin_width);
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        #pragma omp for reduction(+:val), reduction(+:n)
        for (uint64_t i=0; i<nofbins; i++)
        {
            bin_value = (long double)(bin_width*i) + (long double)bin_centre - mean ;
            val += (long double)histogram[i] * powl( bin_value , k);
            n += histogram[i];
        } 
    }
    
    return (double)(val/(long double)n);
}

/*
https://fr.wikipedia.org/wiki/Cumulant_(statistiques)

$$ \kappa_n=m_n-\sum_{k=1}^{n-1} \binom{n-1}{k-1} \, \kappa_k \, m_{n-k} $$
*/

template <class BinType>
double cumulant_k(BinType* histogram, uint64_t nofbins, uint n, double bin_centre, double bin_width)
{
    double ret = moment_k(histogram, nofbins, n, bin_centre, bin_width);
    for (uint64_t k=1; k<n; k++)
    {
        ret -= (double)n_choose_k(n-1, k-1)*cumulant_k(histogram, nofbins, k, bin_centre, bin_width)*moment_k(histogram, nofbins, n-k, bin_centre, bin_width );
    }
    return ret;
}

//////////////////////////////////
// Multivariate 
//////////////////////////////////

template <class BinType>
double moment2D(BinType* histogram, uint64_t nofbins, uint exp_x , uint exp_y , double bin_centre, double bin_width)
{
    long double bin_value = 0;
    long double val = 0;
    uint64_t n=0;
    
    #pragma omp parallel
    {
        manage_thread_affinity(); // For 64+ logical cores on Windows
        #pragma omp for reduction(+:val), reduction(+:n)
        for (uint64_t i=0; i<nofbins; i++)
        {
            bin_value_x = (long double)(bin_width*(i%nofbins)) + (long double)bin_centre ;
            bin_value_y = (long double)(bin_width*(i/nofbins)) + (long double)bin_centre ;
            val += (long double)histogram[i] * powl( bin_value_x , exp_x) * powl( bin_value_y , exp_y)
            n += histogram[i];
        }
    }
    
    return (double)(val/(long double)n);
}


#define INPUTS(EXP_X,EXP_Y) histogram, nofbins, EXP_X , EXP_Y , bin_centre, bin_width 
template <class BinType>
double cumulant_rr(BinType* histogram, uint64_t nofbins, double bin_centre, double bin_width)
{
   
    return moment2D(INPUTS(2,0)) - pow(  moment2D(INPUTS(1,0)) ,2 );
    
}

template <class BinType>
double cumulant_ss(BinType* histogram, uint64_t nofbins, double bin_centre, double bin_width)
{
    
    return moment2D(INPUTS(0,2)) - pow( moment2D(INPUTS(0,1)) , 2 );
    
}

template <class BinType>
double cumulant_rrss(BinType* histogram, uint64_t nofbins, double bin_centre, double bin_width)
{
    
    return moment2D( INPUTS(2,2) ) -  moment2D( INPUTS(2,0) ) * moment2D( INPUTS(0,2) ) - 2.0* pow( moment2D(INPUTS(1,1)) , 2 );
    
}
#undef INPUTS










