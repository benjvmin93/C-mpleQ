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

    struct Matrix *result = init_matrix(*m1->rows, *m2->cols);

    for (size_t i = 0; i < *m1->rows; ++i)
    {
        struct Complex **m1_row = m1->matrix[i];
        struct Complex **res_row = result->matrix[i];
        for (size_t j = 0; j < *m2->cols; ++j)
        {
            for (size_t k = 0; k < *m1->cols; ++k)
            {
                struct Complex **m2_row = m2->matrix[k];

                struct Complex *mul = complex_mul(m1_row[k], m2_row[j]);
                res_row[j] = complex_add(res_row[j], mul);
            }
        }
    }
    return result;

}

bool matrix_equal(struct Matrix *m1, struct Matrix *m2)
{
    if (*m1->rows != *m2->rows || *m1->cols != *m2->cols)
    {
        return false;
    }

    for (size_t i = 0; i < *m1->rows; ++i)
    {
        struct Complex **m1_row = m1->matrix[i];
        struct Complex **m2_row = m2->matrix[i];
        for (size_t j = 0; j < *m1->cols; ++j)
        {
            if (m1_row[j] != m2_row[j])
                return false;
        }
    }
    return true;
}

void print_matrix(struct Matrix *matrix)
{
    for (size_t i = 0; i < *matrix->rows; ++i)
    {
        struct Complex **row = matrix->matrix[i];
        for (size_t j = 0; j < *matrix->cols; ++j)
        {
            struct Complex *c = row[j];
            print_complex(c);
            if (j == *matrix->cols - 1)
            {
                printf("\n");
            }
            else
            {
                printf(" | ");
            }
        }
    }
}

struct Matrix *kron(struct Matrix *m1, struct Matrix *m2) {
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