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
struct Matrix *matrix_add(struct Matrix *m1, struct Matrix *m2);
struct Matrix *matrix_mul(struct Matrix *m1, struct Matrix *m2);
bool matrix_equal(struct Matrix *m1, struct Matrix *m2);
void print_matrix(struct Matrix *matrix);
struct Matrix *kron(struct Matrix *m1, struct Matrix *m2);

#endif
