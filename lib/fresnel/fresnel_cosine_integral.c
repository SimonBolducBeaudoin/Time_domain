////////////////////////////////////////////////////////////////////////////////
// File: fresnel_cosine_integral.c                                            //
// Routine(s):                                                                //
//    Fresnel_Cosine_Integral                                                 //
//    xFresnel_Cosine_Integral                                                //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Note:                                                                     //
//     There are several different definitions of what is called the          //
//     Fresnel cosine integral.  The definition of the Fresnel cosine         //
//     integral, C(x) programmed below is the integral from 0 to x of the     //
//     integrand                                                              //
//                          sqrt(2/pi) cos(t^2) dt.                           //
////////////////////////////////////////////////////////////////////////////////

#include <math.h>           // required for fabsl(), sinl(), cosl()
#include <float.h>          // required for LDBL_EPSILON

//                         Externally Defined Routines                        //
extern long double xFresnel_Auxiliary_Cosine_Integral(long double x);
extern long double xFresnel_Auxiliary_Sine_Integral(long double x);


//                         Internally Defined Routines                        //
double      Fresnel_Cosine_Integral( double x );
long double xFresnel_Cosine_Integral( long double x );

static long double Power_Series_C( long double x );


////////////////////////////////////////////////////////////////////////////////
// double Fresnel_Cosine_Integral( double x )                                 //
//                                                                            //
//  Description:                                                              //
//     The Fresnel cosine integral, C(x), is the integral with integrand      //
//                          sqrt(2/pi) cos(t^2) dt                            //
//     where the integral extends from 0 to x.                                //
//                                                                            //
//  Arguments:                                                                //
//     double  x  The argument of the Fresnel cosine integral C().            //
//                                                                            //
//  Return Value:                                                             //
//     The value of the Fresnel cosine integral C evaluated at x.             //
//                                                                            //
//  Example:                                                                  //
//     double y, x;                                                           //
//                                                                            //
//     ( code to initialize x )                                               //
//                                                                            //
//     y = Fresnel_Cosine_Integral( x );                                      //
////////////////////////////////////////////////////////////////////////////////
double Fresnel_Cosine_Integral( double x )
{
   return (double) xFresnel_Cosine_Integral( (long double) x);
}


////////////////////////////////////////////////////////////////////////////////
// long double xFresnel_Cosine_Integral( long double x )                      //
//                                                                            //
//  Description:                                                              //
//     The Fresnel cosine integral, C(x), is the integral with integrand      //
//                          sqrt(2/pi) cos(t^2) dt                            //
//     where the integral extends from 0 to x.                                //
//                                                                            //
//  Arguments:                                                                //
//     long double  x  The argument of the Fresnel cosine integral C().       //
//                                                                            //
//  Return Value:                                                             //
//     The value of the Fresnel cosine integral C evaluated at x.             //
//                                                                            //
//  Example:                                                                  //
//     long double y, x;                                                      //
//                                                                            //
//     ( code to initialize x )                                               //
//                                                                            //
//     y = xFresnel_Cosine_Integral( x );                                     //
////////////////////////////////////////////////////////////////////////////////

long double xFresnel_Cosine_Integral( long double x )
{
   long double f;
   long double g;
   long double x2;
   long double c;
   
   if ( fabsl(x) < 0.5L) return Power_Series_C(x);

   f = xFresnel_Auxiliary_Cosine_Integral(fabsl(x));
   g = xFresnel_Auxiliary_Sine_Integral(fabsl(x));
   x2 = x * x;
   c = 0.5L + sinl(x2) * f - cosl(x2) * g;
   return ( x < 0.0L) ? -c : c;
}


////////////////////////////////////////////////////////////////////////////////
// static long double Power_Series_C( long double x )                         //
//                                                                            //
//  Description:                                                              //
//     The power series representation for the Fresnel cosine integral, C(x), //
//      is                                                                    //
//                 x sqrt(2/pi) Sum (-x^4)^j / [(4j+1) (2j)!]                 //
//     where the sum extends over j = 0, ,,,.                                 //
//                                                                            //
//  Arguments:                                                                //
//     long double  x                                                         //
//                The argument of the Fresnel cosine integral C().            //
//                                                                            //
//  Return Value:                                                             //
//     The value of the Fresnel cosine integral C evaluated at x.             //
//                                                                            //
//  Example:                                                                  //
//     long double y, x;                                                      //
//                                                                            //
//     ( code to initialize x )                                               //
//                                                                            //
//     y = Power_Series_C( x );                                               //
////////////////////////////////////////////////////////////////////////////////

static long double Power_Series_C( long double x )
{ 
   long double x2 = x * x;
   long double x3 = x * x2;
   long double x4 = - x2 * x2;
   long double xn = 1.0L;
   long double Sn = 1.0L;
   long double Sm1 = 0.0L;
   long double term;
   long double factorial = 1.0L;
   long double sqrt_2_o_pi = 7.978845608028653558798921198687637369517e-1L;
   int y = 0;
   
   if (x == 0.0L) return 0.0L;
   while ( fabsl(Sn - Sm1) > LDBL_EPSILON * fabsl(Sm1) ) {
      Sm1 = Sn;
      y += 1;
      factorial *= (long double)(y + y);
      factorial *= (long double)(y + y - 1);
      xn *= x4;
      term = xn / factorial;
      term /= (long double)(y + y + y + y + 1);
      Sn += term;
   }
   return x * sqrt_2_o_pi * Sn;
}