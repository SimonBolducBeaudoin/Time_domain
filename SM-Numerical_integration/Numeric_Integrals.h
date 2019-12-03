#pragma once
typedef unsigned int uint;

// Methodes de Newton Cotes a 11 points voir : http://mathworld.wolfram.com/Newton-CotesFormulas.html
//      The error is of oder O(h^13) where h = (1/10)*(upper_bound-lower_bound)/N_steps
double Newton_Cotes_11 
    ( 
        double (*Function)(double), // A Pointer to the function that needs to be integrated
        double lower_bound = 0 , // Integrate from that point
        double upper_bound = 1, // Integrate until that point
        uint N_steps = 1000 // The number of newton cote polynominals that are approximating the function
    );