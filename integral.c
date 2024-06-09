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

int main(int argc, char* argv[]) {
   printf("f1(3.92) = %f\n", f1_value(3.92));
   printf("f2(10.76) = %f\n", f2_value(10.76));
   printf("f3(90.2) = %f\n\n", f3_value(90.2));

   printf("f1 ∩ f2 = %f\n", root(f1_value, f2_value, 3, 5, 0.001));
   printf("f1 ∩ f3 = %f\n", root(f1_value, f3_value, 0, 2, 0.001));
   printf("f2 ∩ f3 = %f\n", root(f2_value, f3_value, 2, 4, 0.001));
   return 0;
}
