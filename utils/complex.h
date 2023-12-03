#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdlib.h>

struct Complex
{
    double *a;
    double *b;
};

struct Complex *init_complex(double a, double b);
void free_complex(struct Complex *complex);
struct Complex *complex_add(struct Complex *z1, struct Complex *z2);
struct Complex *complex_mul(struct Complex *z1, struct Complex *z2);
struct Complex *complex_div(struct Complex *z1, struct Complex *z2);
void print_complex(struct Complex *z);

#endif