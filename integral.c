#include <stdio.h>
#include <math.h>
#include <getopt.h>

typedef double afunc(double);

// Functions from ASM
extern double f1_value(double x);
extern double f2_value(double x);
extern double f3_value(double x);

int iterations = 0; // global counter of iterations in root function

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
      ++iterations;
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

// Return function by its sequence number
afunc* func_by_ind(int ind) {
   if (ind == 1) {
      return f1_value;
   }
   else if (ind == 2) {
      return f2_value;
   }
   return f3_value;
}

int main(int argc, char* argv[]) {

   // Roots f(x) = g(x)
   double f12_intersec = root(f1_value, f2_value, 3, 5, 0.001);
   double f13_intersec = root(f1_value, f3_value, 0, 2, 0.001);
   double f23_intersec = root(f2_value, f3_value, 2, 4, 0.001);

   // Integral f(x)
   double f1_integral = integral(f1_value, f13_intersec, f12_intersec, 0.001);
   double f2_integral = integral(f2_value, f23_intersec, f12_intersec, 0.001);
   double f3_integral = integral(f3_value, f13_intersec, f23_intersec, 0.001);

   // Area between curves
   double area = f1_integral - f2_integral - f3_integral;

   // Short options array for command line
   const char* short_options = "hriR:I:"; // ':' means value expectation

   // Long options array for command line
   const struct option long_options[] = {
      // {"option", value_expectation, ptr_on_flag, value_for_flag}
      {"help", no_argument, NULL, 'h'},
      {"root", no_argument, NULL, 'r'},
      {"iterations", no_argument, NULL, 'i'},
      {"test-root", required_argument, NULL, 'R'},
      {"test-integral", required_argument, NULL, 'I'},
      { NULL, 0, NULL, 0}
   };

   char zero_options_flag = 1;
   char option_type;
   int value_index = -1; // index of the parameter string in argv array

   // Scan command line args
   while ((option_type = getopt_long(argc, argv, short_options, long_options, &value_index)) != -1) {
      zero_options_flag = 0;

      switch (option_type) {
         case 'h':
            printf("--help -h \t\t print all avaliable options\n");
            printf("--root -r \t\t print 'x' coordinates of functions intersection\n");
            printf("--iterations -i \t print number of iterations in 'root' function\n");
            printf("--test-root -R \t\t test 'root' function by given argumenst (F1:F2:A:B:E:R)\n");
            printf("--test-integral -I \t test 'integral' function by given arguments (F:A:B:E:R)\n");
            break;

         case 'r':
            printf("f1 ∩ f2 = %f\n", f12_intersec);
            printf("f1 ∩ f3 = %f\n", f13_intersec);
            printf("f2 ∩ f3 = %f\n", f23_intersec);
            break;

         case 'i':
            printf("%i\n", iterations); // global variable
            break;

         // Functions args
         int f_ind, g_ind;
         double a, b, eps;
         double expected_result, function_result, delta;

         case 'R':
            // parse string (optarg - returned by getopt_long string after option)
            sscanf(optarg, "%i:%i:%lf:%lf:%lf:%lf", &f_ind, &g_ind, &a, &b, &eps, &expected_result);

            function_result = root(func_by_ind(f_ind), func_by_ind(g_ind), a, b, eps);
            delta = fabs(expected_result - function_result);

            printf("expected result: %f\n", expected_result);
            printf("function result: %f\n", function_result);
            printf("delta: %f\n", delta);
            printf("abs(delta): %f\n\n", fabs(delta));
            break;

         case 'I':
            // parse string (optarg - returned by getopt_long string after option)
            sscanf(optarg, "%i:%lf:%lf:%lf:%lf", &f_ind, &a, &b, &eps, &expected_result);

            function_result = integral(func_by_ind(f_ind), a, b, eps);
            delta = fabs(function_result - expected_result);

            printf("expected result: %f\n", expected_result);
            printf("function result: %f\n", function_result);
            printf("delta: %f\n", delta);
            printf("abs(delta): %f\n\n", fabs(delta));
            break;

         default:
            printf("Options ERROR");
      };
   }

    // Print area
   if (zero_options_flag) {
      printf("%f\n", area);
   }

   return 0;
}