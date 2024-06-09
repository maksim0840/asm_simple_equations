#include <stdio.h>
#include <math.h>

typedef double afunc(double);

// Functions from ASM
extern double f1_value(double x);
extern double f2_value(double x);
extern double f3_value(double x);


// Find x from f(x)=g(x) by dichotomy algorithm
double root(afunc *f, afunc *g, double a, double b, const double eps1) {
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
   }

   return mid;
}


// Simpson's formule coeficents
struct coefs {
   double first_last_yi; // first(y_i) + last(y_i)
   double even_yi; // sum of all y_i in even positions
   double odd_yi; // sum of all y_i in odd positions

   double step; // step for 'x'
};

// Find integral ∫f(x), x∊[a,b] by Simpson's formule
double simpson(afunc *f, const double a, const double b, struct coefs* coefs) {

   // Pass all 'x' from the [start, end] interval with step
   for (double x = a; x <= b; x += coefs->step) {
      coefs->odd_yi += f(x); // increase odd sum
   }
   
   // (step/3) -> (step/6), because: coefs->step is a step only for getting odd numbers
   return (coefs->step / 6) * (coefs->first_last_yi + 4*coefs->odd_yi + 2*coefs->even_yi);
}


// Starter function for finding the integral using Rugne's rule
double integral(afunc *f, const double a, const double b, const double eps2) {

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


int main(int argc, char* argv[]) {
   printf("f1(3.92) = %f\n", f1_value(3.92));
   printf("f2(10.76) = %f\n", f2_value(10.76));
   printf("f3(90.2) = %f\n\n", f3_value(90.2));

   double f1_f2 = root(f1_value, f2_value, 3, 5, 0.001);
   double f1_f3 = root(f1_value, f3_value, 0, 2, 0.001);
   double f2_f3 = root(f2_value, f3_value, 2, 4, 0.001);

   printf("f1 ∩ f2 = %f\n", f1_f2);
   printf("f1 ∩ f3 = %f\n", f1_f3);
   printf("f2 ∩ f3 = %f\n\n", f2_f3);

   double integral_f1 = integral(f1_value, f1_f3, f1_f2, 0.001);
   double integral_f2 = integral(f2_value, f2_f3, f1_f2, 0.001);
   double integral_f3 = integral(f3_value, f1_f3, f2_f3, 0.001);

   printf("x1=%f, x2=%f, integral(f1) = %f\n", f1_f3, f1_f2, integral_f1);
   printf("x1=%f, x2=%f, integral(f2) = %f\n", f2_f3, f1_f2, integral_f2);
   printf("x1=%f, x2=%f, integral(f3) = %f\n\n", f1_f3, f2_f3, integral_f3);

   double S = integral_f1 - integral_f2 - integral_f3;
   printf("result S(f1, f2, f3) = %f\n", S);

   return 0;
}