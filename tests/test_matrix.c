#include <stdio.h>
#include <assert.h>

#include "../utils/matrix.h"
#include "../utils/complex.h"
#include "tests.h"

#define NB_TESTS_MATRIX 8

/*
    [[1],   [[1],  [[2],        
     [0]] +  [0]] = [0]]
                        
*/
bool test_matrix_1(void)
{
    size_t rows = 2;
    size_t cols = 1;

    struct Matrix *m1 = init_matrix(cols, rows);
    struct Matrix *m2 = init_matrix(cols, rows);
    (*m1->matrix[0][0]->a) = 1;
    (*m2->matrix[0][0]->a) = 1;
    struct Matrix *res = matrix_add(m1, m2);
    struct Matrix *expected = init_matrix(cols, rows);
    *expected->matrix[0][0]->a = 2;

    if (!matrix_equal(res, expected, 0))
        return false;

    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return true;
}

/*
    [[0],   [[0],   [[0],        
     [0]] +  [0]] =  [0]]
                        
*/
bool test_matrix_2(void)
{
    size_t rows = 2;
    size_t cols = 1;

    struct Matrix *m1 = init_matrix(cols, rows);
    struct Matrix *m2 = init_matrix(cols, rows);
    struct Matrix *res = matrix_add(m1, m2);
    struct Matrix *expected = init_matrix(cols, rows);
    
    if (!matrix_equal(res, expected, 0))
        return false;

    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return true;
}

/*
    [[-4 + i],      [[4 - i],    [[0],        
     [-7.4 - i]]  +  [0 + i]]  =  [-7.4]]
                        
*/
bool test_matrix_3(void)
{
    size_t rows = 2;
    size_t cols = 1;

    
    struct Matrix *m1 = init_matrix(cols, rows);
    *m1->matrix[0][0]->a = -4;
    *m1->matrix[0][0]->b = 1;
    *m1->matrix[1][0]->a = -7.4;
    *m1->matrix[1][0]->b = -1;

    struct Matrix *m2 = init_matrix(cols, rows);
    *m2->matrix[0][0]->a = 4;
    *m2->matrix[0][0]->b = -1;
    *m2->matrix[1][0]->b = 1;

    struct Matrix *expected = init_matrix(cols, rows);
    *expected->matrix[1][0]->a = -7.4;
    
    struct Matrix *res = matrix_add(m1, m2);
    
    if (!matrix_equal(res, expected, 0))
        return false;
    

    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return true;
}

/*
    [[0, 1],     [[1]     [[0],        
     [1, 0]]  *   [0]  =   [1]]
                        
*/
bool test_matrix_4(void)
{    
    struct Matrix *m1 = init_matrix(2, 2);
    *m1->matrix[0][1]->a = 1;
    *m1->matrix[1][0]->a = 1;

    struct Matrix *m2 = init_matrix(1, 2);
    *m2->matrix[0][0]->a = 1;

    struct Matrix *expected = init_matrix(1, 2);
    *expected->matrix[1][0]->a = 1;
    
    struct Matrix *res = matrix_mul(m1, m2);

    bool b = matrix_equal(res, expected, 0);
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return b;
}

/*
                        [[1],        
   [[1],     [[1],   =   [0],
    [0]]  ⊗  [0]]       [0],
                         [0]]
                        
*/
bool test_matrix_5(void)
{
    struct Matrix *m1 = init_matrix(1, 2);
    *m1->matrix[0][0]->a = 1;

    struct Matrix *m2 = init_matrix(1, 2);
    *m2->matrix[0][0]->a = 1;

    struct Matrix *expected = init_matrix(1, 4);
    *expected->matrix[0][0]->a = 1;

    struct Matrix *res = matrix_kron(m1, m2);

    bool b = matrix_equal(res, expected, 0);
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return b;
}

/*
                        [[0],        
   [[0],     [[0],   =   [0],
    [1]]  ⊗  [1]]       [0],
                         [1]]
                        
*/
bool test_matrix_6(void)
{
    struct Matrix *m1 = init_matrix(1, 2);
    *m1->matrix[1][0]->a = 1;

    struct Matrix *m2 = init_matrix(1, 2);
    *m2->matrix[1][0]->a = 1;

    struct Matrix *expected = init_matrix(1, 4);
    *expected->matrix[3][0]->a = 1;

    struct Matrix *res = matrix_kron(m1, m2);

    bool b = matrix_equal(res, expected, 0);
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return b;
}

/*
                                [[1, 0, 0, 0],
   [[1, 0]       [[1, 0],        [0, 1, 0, 0],
    [0, 1]]  ⊗   [0, 1]]   =    [0, 0, 1, 0],
                                 [0, 0, 0, 1]]
*/
bool test_matrix_7(void)
{
    struct Matrix *m1 = init_matrix(2, 2);
    *m1->matrix[0][0]->a = 1;
    *m1->matrix[1][1]->a = 1;

    struct Matrix *m2 = init_matrix(2, 2);
    *m2->matrix[0][0]->a = 1;
    *m2->matrix[1][1]->a = 1;

    struct Matrix *expected = init_matrix(4, 4);
    *expected->matrix[0][0]->a = 1;
    *expected->matrix[1][1]->a = 1;
    *expected->matrix[2][2]->a = 1;
    *expected->matrix[3][3]->a = 1;

    struct Matrix *res = matrix_kron(m1, m2);

    bool b = matrix_equal(res, expected, 0);
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return b;
}

/*
              T     
   [[1, 2, 3]       [[1, 4],
    [4, 5, 6]]  =    [2, 5],
                     [3, 6]]    
*/
bool test_matrix_8(void)
{
    struct Matrix *m = init_matrix(3, 2);
    double k = 1;
    for (size_t i = 0; i < *m->rows; ++i)
    {
        for (size_t j = 0; j < *m->cols; ++j)
        {
            matrix_set_complex(m, k++, 0, j, i);
        }
    }

    struct Matrix *transposed = matrix_transpose(m);
    bool b = true;
    for (size_t i = 0; i < *transposed->rows; ++i)
    {
        for (size_t j = 0; j < *transposed->cols; ++j)
        {
            b = complex_equal(transposed->matrix[i][j], m->matrix[j][i], 0);
            if (b == false)
                break;
        }
        if (b == false)
            break;
    }
    free_matrix(transposed);
    free_matrix(m);
    return b;
}

bool (*matrix_test_functions[])(void) = {
    test_matrix_1,
    test_matrix_2,
    test_matrix_3,
    test_matrix_4,
    test_matrix_5,
    test_matrix_6,
    test_matrix_7,
    test_matrix_8
};

void run_tests_matrix(void)
{
    printf("========== TESTS MATRIX ==========\n");
    float success = 0;
    for (size_t i = 0; i < NB_TESTS_MATRIX; ++i)
    {
        printf("Test %ld: ", i + 1);
        bool res = (*matrix_test_functions[i])();
        success += res == true ? 1 : 0;
        if (res)
            printf("PASSED\n");
        else
            printf("FAILED\n");
    }

    printf("Matrix tests passed = %f%%\n", success / (float)NB_TESTS_MATRIX * 100);
}