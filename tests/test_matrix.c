#include <stdio.h>
#include <assert.h>

#include "../utils/matrix.h"
#include "../utils/complex.h"
#include "tests.h"

#define NB_TESTS_MATRIX 7

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

    if (!matrix_equal(res, expected))
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
    
    if (!matrix_equal(res, expected))
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
    
    if (!matrix_equal(res, expected))
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

    bool b = matrix_equal(res, expected);
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

    struct Matrix *res = kron(m1, m2);

    bool b = matrix_equal(res, expected);
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

    struct Matrix *res = kron(m1, m2);

    bool b = matrix_equal(res, expected);
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

    struct Matrix *res = kron(m1, m2);

    bool b = matrix_equal(res, expected);
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(res);
    free_matrix(expected);
    return b;
}

bool (*test_functions[])(void) = {
    test_matrix_1,
    test_matrix_2,
    test_matrix_3,
    test_matrix_4,
    test_matrix_5,
    test_matrix_6,
    test_matrix_7
};

void run_tests_matrix(void)
{
    printf("========== TESTS MATRIX ==========\n");
    float success = 0;
    for (size_t i = 0; i < NB_TESTS_MATRIX; ++i)
    {
        printf("Test %ld: ", i + 1);
        bool res = (*test_functions[i])();
        success += res == true ? 1 : 0;
        if (res)
            printf("PASSED\n");
        else
            printf("FAILED\n");
        
    }

    printf("Matrix tests passed = %f%%\n", success / (float)NB_TESTS_MATRIX * 100);
}