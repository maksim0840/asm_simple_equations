#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <math.h>
#include "src/calc_funcs.h"

// Functions from ASM
extern double f1_value(double);
extern double f2_value(double);
extern double f3_value(double);

// Functions from test
extern double f1_test(double);
extern double f2_test(double);
extern double f3_test(double);

// Global value from calculus functions
extern int iterations;



// Return function by its sequence number
afunc* func_by_ind(int ind) {
   switch (ind) {
      case(1):
         return f1_value;
      case(2):
         return f2_value;
      case(3):
         return f3_value;
      case(4):
         return f1_test;
      case(5):
         return f2_test;
      case(6):
         return f3_test;
      default:
         printf("Function ERROR");
         exit(-1);
   }
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
            exit(-1);
      };
   }

    // Print area
   if (zero_options_flag) {
      printf("%f\n", area);
   }

   return 0;
}