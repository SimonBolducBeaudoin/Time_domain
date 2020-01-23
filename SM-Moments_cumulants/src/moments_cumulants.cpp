#include "../includes/moments_cumulants.h"
#include "../src/moments_cumulants.tpp"

// Explicit instantiation
// see: https://en.cppreference.com/w/cpp/language/function_template
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



