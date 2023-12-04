#include <stdio.h>

#include "../utils/matrix.h"
#include "../utils/complex.h"
#include "tests.h"

#define NB_TESTS 5

void test_matrix(void)
{
    struct Matrix *m1 = init_matrix(1, 2);
    struct Matrix *m2 = init_matrix(1, 2);
    (*m1->matrix[0][0]->a) = 1;
    (*m2->matrix[0][0]->a) = 1;

    struct Matrix *res = matrix_add(m1, m2);

    print_matrix(res);

    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
}