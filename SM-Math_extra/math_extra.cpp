#include "math_extra.h"

uint64_t n_choose_k(uint n, uint k)
{
    if (k==0)
    {
        return 1;
    }
    return (n*n_choose_k(n-1, k-1))/k; // Product form, division always yields an integer
}