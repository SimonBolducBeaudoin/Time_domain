#pragma once

#include "../../includes/header_common.h" 
#include "../../SM-Math_extra/math_extra.h"
#include "../../SM-Omp_extra/includes/omp_extra.h"

template <class BinType>
double moment_k(BinType* histogram, uint64_t nofbins, uint k, double bin_centre = 0 , double bin_width = 1.0);

template <class BinType>
double centered_moment_k(BinType* histogram, uint64_t nofbins, uint k, double bin_centre = 0 , double bin_width = 1.0);

template <class BinType>
double cumulant_k(BinType* histogram, uint64_t nofbins, uint k, double bin_centre = 0 , double bin_width = 1.0);

/**
 * moment_k - return the kth moment of an 1D histogram defined by histogram and nofbins.
 *
 *  VARIALBLES :
 *      histogram is a pointer to a 1D histogram
 *      nofbins is the number of element in the pointer's list
 *      k is the order of the moment that must be calculated
 *      bin_centre is the abscisse value of the first element in the list (i.e. the center of the 1st bin)
 *      bin_width is the width of a bin
 *
 *  If bin_centre and bin_width are not used then the function defaults to uint type histogram.  
 *
 *
 *  Examples : 
        moment_k ( ... , ... , 1 ) returns the mean
            and uses bin_values =  1.0 * i + 0 ; 
        
        moment_k ( ... , ... , 1, 0.5 ) returns the mean
            and uses bin_values =  1.0 * i + 0.5 ; 
        
        moment_k ( ... , ... , 1, 0.5 , 2.0 ) returns the mean
            and uses bin_values =  1.0 * i + bin_centre ; 
        
        moment_k ( ... , ... , 2 ) returns the variance
        
        centered_moment_k ( ... , ... , 1 ) returns the centered mean (always 0)
        centered_moment_k ( ... , ... , 2 ) returns the centered variance
 * 
 */





