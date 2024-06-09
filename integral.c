#include <stdio.h>
#include <math.h>

// Functions from ASM
extern double f1_value(double x);
extern double f2_value(double x);
extern double f3_value(double x);

// Find x from f1(x)=f2(x) by dichotomy algorithm
double root(double (*f1)(double), double(*f2)(double), double a, double b, const double eps) {
   double mid = (a + b) / 2; // number in the middle of the segment [a, b]

   while (fabs(f1(mid) - f2(mid)) > eps) {

      // Check the signs at the ends of the segment
      if ((f1(a) - f2(a)) * (f1(mid) - f2(mid)) <= 0) {
         b = mid; // 0 is included in [a, mid]
      }
      else {
         a = mid; // 0 is included in [mid, b]
      }

      mid = (a + b) / 2;
   }

   return mid;
}

// find integral ∫f(x), x∊[a,b] by Simpson's formule
double integral(double(*f)(double), const double a, const double b, const double eps) {

   double first_last_yi = 0; // first(y_i) + last(y_i)
   double even_yi = 0; // sum of all y_i in even positions
   double odd_yi = 0; // sum of all y_i in odd positions

   int i = 0;

   // Pass all 'x' from the [a,b] interval with step='eps'
   for (double x = a; x <= b; x += eps) {

      double y_i = f(x);

      if (i == 0) { // first (i)
         first_last_yi += y_i;
      }
      else if (x + eps > b) { // last (i)
         first_last_yi += y_i;
      }
      else if (i % 2 == 0) { // even (i)
         even_yi += y_i;
      }
      else { // odd (i)
         odd_yi += y_i;
      }

      ++i;
   }

   return (eps / 3) * (first_last_yi + 4*odd_yi + 2*even_yi);
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

   double integral_f1 = integral(f1_value, f1_f3, f1_f2, 0.00001);
   double integral_f2 = integral(f2_value, f2_f3, f1_f2, 0.00001);
   double integral_f3 = integral(f3_value, f1_f3, f2_f3, 0.00001);

   printf("x1=%f, x2=%f, integral(f1) = %f\n", f1_f3, f1_f2, integral_f1);
   printf("x1=%f, x2=%f, integral(f2) = %f\n", f2_f3, f1_f2, integral_f2);
   printf("x1=%f, x2=%f, integral(f3) = %f\n\n", f1_f3, f2_f3, integral_f3);

   double S = integral_f1 - integral_f2 - integral_f3;
   printf("result S(f1, f2, f3) = %f\n", S);

   return 0;
}
