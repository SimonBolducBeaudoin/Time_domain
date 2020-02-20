#pragma once

#include "../../includes/header_common.h" 
#include "../../SM-Math_extra/math_extra.h"
#include "../../SM-Omp_extra/includes/omp_extra.h"

//////////////////////////////////
// Monovariate
//////////////////////////////////

/*
moment_k - return the kth moment of an 1D histogram defined by histogram and nofbins.
 
 VARIALBLES :
     histogram is a pointer to a 1D histogram
     nofbins is the number of element in the pointer's list
     k is the order of the moment that must be calculated
     first_bin_center is the abscisse value of the first element in the list (i.e. the center of the 1st bin)
     bin_width is the width of a bin
     
 If first_bin_center and bin_width are not used then the function defaults to uint type histogram.  

 Examples : 
        moment_k ( ... , ... , 1 ) returns the mean
            and uses bin_values =  1.0 * i + 0 ; 
        
        moment_k ( ... , ... , 1, 0.5 ) returns the mean
            and uses bin_values =  1.0 * i + 0.5 ; 
        
        moment_k ( ... , ... , 1, 0.5 , 2.0 ) returns the mean
            and uses bin_values =  1.0 * i + first_bin_center ; 
        
        moment_k ( ... , ... , 2 ) returns the variance
        
        centered_moment_k ( ... , ... , 1 ) returns the centered mean (always 0)
        centered_moment_k ( ... , ... , 2 ) returns the centered variance

 */

template <class BinType>
double moment_k(BinType* histogram, uint64_t nofbins, uint k, double first_bin_center = 0 , double bin_width = 1.0);

template <class BinType>
double centered_moment_k(BinType* histogram, uint64_t nofbins, uint k, double first_bin_center = 0 , double bin_width = 1.0);

template <class BinType>
double cumulant_k(BinType* histogram, uint64_t nofbins, uint k, double first_bin_center = 0 , double bin_width = 1.0);

//////////////////////////////////
// Multivariate 
//////////////////////////////////
/*
WARNING : This as not been implemented for a general multivariate case.
          It also not writen to be efficient at all.

I've only implemented some particular cases for multivariate cumulant.
Recursives relation for the general case can be found in the following references,
but it seemed to complicated to implement for now.

Also I assume histograms to squared and to be 2D and I assume that distribution is centered (first moment = 0)

Cases implemented  :
    - moment2D(exp_x = 1 ) == expval ( X )
    - moment2D(exp_y = 1 ) == expval ( Y )
    - moment2D(exp_x = 1 , exp_y = 1 ) == expval ( XY )
    - moment2D(exp_x = 2 , exp_y = 2 ) == expval ( X^2 Y^2 )
    - cumulant_ii
    - cumulant_jj
    - cumulant_iijj

Ref:
http://www.stat.uchicago.edu/~pmcc/courses/stat306/2017/cumulants.pdf
https://link.springer.com/content/pdf/10.1007/s11004-009-9258-9.pdf
*/

template <class BinType>
double moment2D(BinType* histogram, uint64_t nofbins, uint exp_x = 0 , uint exp_y = 0 , double first_bin_center = 0 , double bin_width = 1.0);

template <class BinType>
double cumulant_ii(BinType* histogram, uint64_t nofbins, double first_bin_center = 0 , double bin_width = 1.0);

template <class BinType>
double cumulant_jj(BinType* histogram, uint64_t nofbins, double first_bin_center = 0 , double bin_width = 1.0);

template <class BinType>
double cumulant_iijj(BinType* histogram, uint64_t nofbins, double first_bin_center = 0 , double bin_width = 1.0);







