#include <math.h>

// f1 = 1 + 4 / (x^2 + 1)
double f1_test(double x) {
	return 1 + 4 / (pow(x, 2) + 1);
}

// f2 = x^3
double f2_test(double x) {
	return pow(x, 3);
}

// f3 = 2^(-x)
double f3_test(double x) {
 return pow(2, -1*x);
}
