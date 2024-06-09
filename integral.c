#include <stdio.h>

extern double f1_value(double x);
extern double f2_value(double x);
extern double f3_value(double x);

int main(int argc, char* argv[]) {
   printf("%f\n", f1_value(5.4));
   printf("%f\n", f2_value(5.4));
   printf("%f\n", f3_value(5.4));
   return 0;
}
