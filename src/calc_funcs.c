#include <math.h>
#include "calc_funcs.h"


int iterations = 0; // global counter of iterations in root function


// Find x from f(x)=g(x) by dichotomy algorithm
double root(afunc* f, afunc* g, double a, double b, const double eps1) {

   double mid = (a + b) / 2; // number in the middle of the segment [a, b]

   while (fabs(f(mid) - g(mid)) > eps1) {

      // Check the signs at the ends of the segment
      if ((f(a) - g(a)) * (f(mid) - g(mid)) <= 0) {
         b = mid; // 0 is included in [a, mid]
      }
      else {
         a = mid; // 0 is included in [mid, b]
      }

      mid = (a + b) / 2;
      ++iterations;
   }

   return mid;
}


// Find integral ∫f(x), x∊[a,b] by Simpson's formule
double simpson(afunc* f, const double a, const double b, struct coefs* coefs) {

   // Pass all 'x' from the [start, end] interval with step
   for (double x = a; x <= b; x += coefs->step) {
      coefs->odd_yi += f(x); // increase odd sum
   }
   
   // (step/3) -> (step/6), because: coefs->step is a step only for getting odd numbers
   return (coefs->step / 6) * (coefs->first_last_yi + 4*coefs->odd_yi + 2*coefs->even_yi);
}


// Starter function for finding the integral using Rugne's rule
double integral(afunc* f, const double a, const double b, const double eps2) {

   struct coefs coefs = {0, 0, 0, 0}; // create and null simspon's coefs
   coefs.step = (b - a) / 5; // determine step for 'x'
   coefs.first_last_yi = f(a) + f(b); // determine sum of first and last y_i

   double integ_iter1; // integral calculated by "n" iterations
   double integ_iter2; // integral calculated by "2n" iterations

   simpson(f, a + coefs.step, b - coefs.step, &coefs); // calculate values in odd positions
   
   /* When step is decreasing by 2, 
      between old values appear new ones in odd positions */

   do {
      coefs.even_yi += coefs.odd_yi; // mark odd positions as even positions
      coefs.odd_yi = 0; // null odd posiotons
      // Move 'start', 'end' for half and calculate values in odd positions
      integ_iter1 = simpson(f, a + coefs.step/2, b - coefs.step/2, &coefs);
      coefs.step /= 2;

      coefs.even_yi += coefs.odd_yi;
      coefs.odd_yi = 0;
      integ_iter2 = simpson(f, a + coefs.step/2, b - coefs.step/2, &coefs);
      coefs.step /= 2;
   }
   while (fabs(integ_iter1 - integ_iter2) / 15 >= eps2);

   // Integral with 'eps' accuracy = I₂ₙ when (|Iₙ - I₂ₙ|/15 < eps)
   return integ_iter2;
}
