#include <err.h>

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

    matrix->matrix = xmalloc(rows * sizeof(void **));
    for (size_t i = 0; i < rows; ++i)
    {
        void **row = matrix->matrix[i];
        row = xmalloc(cols * sizeof(struct Complex *));
        for (size_t j = 0; j < cols; ++j)
        {
            struct Complex *complex = init_complex(0, 0);
            row[j] = complex;
        }
    }
    return matrix;
}
void free_matrix(struct Matrix *matrix)
{
    for (size_t i = 0; i < *matrix->rows; ++i)
    {
        struct Complex **row = matrix->matrix[i];
        for (size_t j = 0; j < *matrix->cols; ++j)
        {
            struct Complex *complex = row[j];
            free_complex(complex);
        }
        free(row);
    }

    free(matrix->matrix);
    free(matrix->cols);
    free(matrix->rows);
}

struct Matrix *matrix_add(struct Matrix *m1, struct Matrix *m2)
{
    struct Matrix *result = init_matrix(*m1->cols, *m1->rows);

    for (size_t i = 0; i < *result->rows; ++i)
    {
        struct Complex **res_row = result->matrix[i];
        struct Complex **m1_row = m1->matrix[i];
        struct Complex **m2_row = m2->matrix[i];
        for (size_t j = 0; j < *result->cols; ++j)
        {
            res_row[j] = complex_add(m1_row[j], m2_row[j]);
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