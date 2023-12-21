#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdbool.h>

struct Matrix
{
    size_t *cols;
    size_t *rows;
    struct Complex ***matrix;
};

struct Matrix *init_matrix(size_t cols, size_t rows);
void free_matrix(struct Matrix *matrix);
struct Matrix *matrix_set_complex(struct Matrix *m, double a, double b, size_t col, size_t row);
struct Matrix *matrix_add(struct Matrix *m1, struct Matrix *m2);
struct Matrix *matrix_mul(struct Matrix *m1, struct Matrix *m2);
struct Matrix *matrix_scal_mul(struct Complex *z, struct Matrix *m);
bool matrix_equal(struct Matrix *m1, struct Matrix *m2, double threshold);
void print_matrix(struct Matrix *matrix);
struct Matrix *matrix_kron(struct Matrix *m1, struct Matrix *m2);
struct Matrix *matrix_transpose(struct Matrix *m);
struct Matrix *identity(size_t n);

#endif
