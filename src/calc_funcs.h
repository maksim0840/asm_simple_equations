#pragma once

typedef double afunc(double);

double root(afunc*, afunc*, double, double, const double);

// Simpson's formule coeficents
struct coefs {
   double first_last_yi; // first(y_i) + last(y_i)
   double even_yi; // sum of all y_i in even positions
   double odd_yi; // sum of all y_i in odd positions

   double step; // step for 'x'
};

double simpson(afunc*, const double, const double, struct coefs*);

double integral(afunc*, const double, const double, const double);