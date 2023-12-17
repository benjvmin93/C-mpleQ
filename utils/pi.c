#include "pi.h"

double calculate_PI(size_t terms)
{
    double pi = 0.0;
    int sign = 1;

    for (size_t i = 0; i < terms; ++i) {
        pi += sign / (2.0 * i + 1);
        sign = -sign;  // Flip the sign for the next term
    }

    return pi * 4.0;
}