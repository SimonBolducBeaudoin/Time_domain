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