#include <math.h>
#include <stdio.h>

#include "complex.h"
#include "alloc.h"

struct Complex *init_complex(double a, double b)
{
    struct Complex *complex = xmalloc(sizeof(struct Complex));
    complex->a = xmalloc(sizeof(double));
    complex->b = xmalloc(sizeof(double));
    *(complex->a) = a;
    *(complex->b) = b;

    return complex; 
}

void free_complex(struct Complex *complex)
{
    free(complex->a);
    free(complex->b);
    free(complex);
}

struct Complex *complex_add(struct Complex *z1, struct Complex *z2)
{
    double a = *z1->a;
    double b = *z1->b;
    double c = *z2->a;
    double d = *z2->b;

    double e = a + c;
    double f = b + d;

    return init_complex(e, f);
}

struct Complex *complex_mul(struct Complex *z1, struct Complex *z2)
{   
    double a = *z1->a;
    double b = *z1->b;
    double c = *z2->a;
    double d = *z2->b;

    double e = a * c - b * d;
    double f = a * d + b * c;

    return init_complex(e, f);
}

struct Complex *complex_div(struct Complex *z1, struct Complex *z2)
{
    double a = *z1->a;
    double b = *z1->b;
    double c = *z2->a;
    double d = *z2->b;

    double squared = pow(c, 2) + pow(d, 2);
    double e = (a * c + b * d) / squared;
    double f = (b * c - a * d) / squared;

    return init_complex(e, f);
}

void print_complex(struct Complex *z)
{
    printf("%f + i%f\n", *z->a, *z->b);
}