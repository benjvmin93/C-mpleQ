#include <err.h>
#include <stdio.h>

#include "matrix.h"
#include "alloc.h"
#include "complex.h"

struct Matrix *init_matrix(size_t cols, size_t rows)
{
    struct Matrix *matrix = xmalloc(sizeof(struct Matrix));
    matrix->cols = xmalloc(sizeof(size_t));
    matrix->rows = xmalloc(sizeof(size_t));
    *matrix->cols = cols;
    *matrix->rows = rows;

    matrix->matrix = (struct Complex ***) xmalloc(rows * sizeof(struct Complex **));
    for (size_t i = 0; i < rows; ++i)
    {
        matrix->matrix[i] = (struct Complex **) xmalloc(cols * sizeof(struct Complex*));
        for (size_t j = 0; j < cols; ++j)
        {
            matrix->matrix[i][j] = init_complex(0, 0);
        }
    }
    return matrix;
}

void free_matrix(struct Matrix *matrix)
{
    for (size_t i = 0; i < *matrix->rows; ++i)
    {
        for (size_t j = 0; j < *matrix->cols; ++j)
        {
            struct Complex *c = matrix->matrix[i][j];
            if (c)
            {
                free_complex(c);
            }
        }
        free(matrix->matrix[i]);
    }

    free(matrix->cols);
    free(matrix->rows);
    free(matrix->matrix);
    free(matrix);
}

struct Matrix *matrix_set_complex(struct Matrix *m, double a, double b, size_t col, size_t row)
{
    if (col > *(m->cols) - 1 || row > *(m->rows) - 1)
    {
        err(1, "Invalid matrix range");
    }
    
    *m->matrix[row][col]->a = a;
    *m->matrix[row][col]->b = b;
    return m;

}

struct Matrix *matrix_add(struct Matrix *m1, struct Matrix *m2)
{
    struct Matrix *result = init_matrix(*m1->cols, *m1->rows);

    for (size_t i = 0; i < *result->rows; ++i)
    {
        for (size_t j = 0; j < *result->cols; ++j)
        {
            if (result->matrix[i][j])
                free_complex(result->matrix[i][j]);
            result->matrix[i][j] = complex_add(m1->matrix[i][j], m2->matrix[i][j]);
        }
    }

    return result;
}

struct Matrix *matrix_mul(struct Matrix *m1, struct Matrix *m2)
{
    if (!(*m1->cols == *m2->rows))
    {
        err(1, "matrix_mul: m1->cols != m2->rows");
    }

    struct Matrix *result = init_matrix(*m2->cols, *m1->rows);

    for (size_t i = 0; i < *m1->rows; ++i)
    {
        for (size_t j = 0; j < *m2->cols; ++j)
        {
            for (size_t k = 0; k < *m1->cols; ++k)
            {
                struct Complex *mul = complex_mul(m1->matrix[i][k], m2->matrix[k][j]);
                struct Complex *add = complex_add(result->matrix[i][j], mul);
                free_complex(mul);
                *result->matrix[i][j]->a = *add->a;
                *result->matrix[i][j]->b = *add->b;
                free_complex(add);

            }
        }
    }
    return result;

}

bool matrix_equal(struct Matrix *m1, struct Matrix *m2, double threshold)
{
    if (*m1->rows != *m2->rows || *m1->cols != *m2->cols)
    {
        return false;
    }

    for (size_t i = 0; i < *m1->rows; ++i)
    {
        for (size_t j = 0; j < *m1->cols; ++j)
        {
            bool equal = complex_equal(m1->matrix[i][j], m2->matrix[i][j], threshold);
            if (!equal)
            {
                return false;
            }
        }
    }
    return true;
}

struct Matrix *matrix_scal_mul(struct Complex *z, struct Matrix *m)
{
    for (size_t i = 0; i < *m->rows; ++i)
    {
        for (size_t j = 0; j < *m->cols; ++j)
        {
            struct Complex *c = complex_mul(z, m->matrix[i][j]);
            m = matrix_set_complex(m, *c->a, *c->b, j, i);
            free_complex(c);
        }
    }
    return m;
}

void print_matrix(struct Matrix *matrix)
{
    printf("[");
    for (size_t i = 0; i < *matrix->rows; ++i)
    {
        if (i > 0)
            printf(" ");
        printf("[ ");
        for (size_t j = 0; j < *matrix->cols; ++j)
        {
            print_complex(matrix->matrix[i][j]);
            if (j == *matrix->cols - 1)
                printf(" ]");
            else
                printf(", ");
        }
        if (i == *matrix->rows - 1)
            printf("]\n");
        else
            printf("\n");
    }
}

struct Matrix *matrix_kron(struct Matrix *m1, struct Matrix *m2) {
    size_t rows = *(m1->rows) * *(m2->rows);
    size_t cols = *(m1->cols) * *(m2->cols);

    // Initialize the result matrix
    struct Matrix *result = init_matrix(cols, rows);

    // Compute the Kronecker product
    for (size_t i = 0; i < *(m1->rows); i++) {
        for (size_t j = 0; j < *(m1->cols); j++) {
            for (size_t p = 0; p < *(m2->rows); p++) {
                for (size_t q = 0; q < *(m2->cols); q++) {
                    *(result->matrix[i * *(m2->rows) + p][j * *(m2->cols) + q]->a) =
                        *(m1->matrix[i][j]->a) * *(m2->matrix[p][q]->a) - *(m1->matrix[i][j]->b) * *(m2->matrix[p][q]->b);

                    *(result->matrix[i * *(m2->rows) + p][j * *(m2->cols) + q]->b) =
                        *(m1->matrix[i][j]->a) * *(m2->matrix[p][q]->b) + *(m1->matrix[i][j]->b) * *(m2->matrix[p][q]->a);
                }
            }
        }
    }

    return result;
}

struct Matrix *matrix_transpose(struct Matrix *m)
{
    struct Matrix *transposed = init_matrix(*m->rows, *m->cols);
    for (size_t i = 0; i < *m->cols; ++i)
    {
        for (size_t j = 0; j < *m->rows; ++j)
        {
            *transposed->matrix[i][j]->a = *m->matrix[j][i]->a;
            *transposed->matrix[i][j]->b = *m->matrix[j][i]->b;
        }
    }
    return transposed;
}

struct Matrix *identity(size_t n)
{
    struct Matrix *identity = init_matrix(n, n);
    for (size_t i = 0; i < n; ++i)
    {
        identity = matrix_set_complex(identity, 1, 0, i, i);
    }
    return identity;
}