#include "../includes/moments_cumulants.h"
#include "../src/moments_cumulants.tpp"

// Explicit instantiation
// see: https://en.cppreference.com/w/cpp/language/function_template

//////////////////////////////////
// Monovariate
//////////////////////////////////
template double moment_k(uint64_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width );
template double moment_k(uint32_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width );
template double moment_k(uint16_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width );
template double moment_k(uint8_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width );

template double centered_moment_k(uint64_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width );
template double centered_moment_k(uint32_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width );
template double centered_moment_k(uint16_t* histogram, uint64_t nofbins, uint k,double bin_centre , double bin_width );
template double centered_moment_k(uint8_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width );

template double cumulant_k(uint64_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width);
template double cumulant_k(uint32_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width);
template double cumulant_k(uint16_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width);
template double cumulant_k(uint8_t* histogram, uint64_t nofbins, uint k, double bin_centre , double bin_width);

//////////////////////////////////
// Multivariate 
//////////////////////////////////

template double moment2D(uint64_t* histogram, uint64_t nofbins, uint exp_x , uint exp_y , double bin_centre , double bin_width );
template double moment2D(uint32_t* histogram, uint64_t nofbins, uint exp_x , uint exp_y , double bin_centre , double bin_width );
template double moment2D(uint16_t* histogram, uint64_t nofbins, uint exp_x , uint exp_y , double bin_centre , double bin_width );
template double moment2D(uint8_t* histogram, uint64_t nofbins, uint exp_x , uint exp_y , double bin_centre , double bin_width );

template double cumulant_rr(uint64_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_rr(uint32_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_rr(uint16_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_rr(uint8_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );

template double cumulant_ss(uint64_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_ss(uint32_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_ss(uint16_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_ss(uint8_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );

template double cumulant_rrss(uint64_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_rrss(uint32_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_rrss(uint16_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );
template double cumulant_rrss(uint8_t* histogram, uint64_t nofbins, double bin_centre , double bin_width );



