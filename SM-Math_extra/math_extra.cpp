#include "math_extra.h"

uint64_t n_choose_k(uint n, uint k)
{
    if (k==0)
    {
        return 1;
    }
    return (n*n_choose_k(n-1, k-1))/k; // Product form, division always yields an integer
}


/*
from : https://www.geeksforgeeks.org/check-if-given-number-is-perfect-square-in-cpp/
*/
bool isPerfectSquare(long double x) 
{   
  // Find floating point value of  
  // square root of x. 
  long double sr = sqrt(x); 
  
  // If square root is an integer 
  return ((sr - floor(sr)) == 0); 
} 