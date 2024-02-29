#include <stdio.h>
#include <math.h>

#include "../utils/maths.h"
#include "../utils/complex.h"
#include "../emulator/gates.h"

bool test_gate_Rx(void)
{
    printf("RX: ");
    double theta = M_PI;
    struct Gate *Rx = init_gate(RX, theta, NULL, 0);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 0, 0);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 1, 1);
    expected = matrix_set_complex(expected, 0, -msin(theta / 2), 1, 0);
    expected = matrix_set_complex(expected, 0, -msin(theta / 2), 0, 1);

    bool res = matrix_equal(Rx->m_gate, expected, 0);

    free_gate(Rx);
    free_matrix(expected);
    return res;
}

bool test_gate_Ry(void)
{
    printf("RY: ");
    double theta = M_PI;
    struct Gate *Ry = init_gate(RY, theta, NULL, 0);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 0, 0);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 1, 1);
    expected = matrix_set_complex(expected, -msin(theta / 2), 0, 1, 0);
    expected = matrix_set_complex(expected, msin(theta / 2), 0, 0, 1);

    bool res = matrix_equal(Ry->m_gate, expected, 0);

    free_gate(Ry);
    free_matrix(expected);
    return res;
}

bool test_gate_Rz(void)
{
    printf("RZ: ");
    double theta = M_PI;
    struct Gate *Rz = init_gate(RZ, theta, NULL, 0);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, mcos(theta / 2), -msin(theta / 2), 0, 0);
    expected = matrix_set_complex(expected, mcos(theta / 2), msin(theta / 2), 1, 1);

    bool res = matrix_equal(Rz->m_gate, expected, 0);

    free_gate(Rz);
    free_matrix(expected);
    return res;
}

bool test_gate_X(void)
{
    printf("X: ");
    struct Matrix *X = get_X();
    struct Matrix *expected = init_matrix(2, 2);
    
    expected = matrix_set_complex(expected, 1, 0, 1, 0);
    expected = matrix_set_complex(expected, 1, 0, 0, 1);
    
    bool res = matrix_equal(X, expected, 0);
    free_matrix(X);
    free_matrix(expected);
    return res;
}

bool test_gate_H(void)
{
    printf("H: ");
    struct Matrix *H = get_H();
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, 1, 0, 0, 0);
    expected = matrix_set_complex(expected, 1, 0, 1, 0);
    expected = matrix_set_complex(expected, 1, 0, 0, 1);
    expected = matrix_set_complex(expected, -1, 0, 1, 1);
    struct Complex *c = init_complex(1 / sqrt(2), 0);
    expected = matrix_scal_mul(c, expected);
    
    bool res = matrix_equal(H, expected, 1e-10);

    free_complex(c);
    free_matrix(H);
    free_matrix(expected);
    return res;
}

bool test_gate_Y(void)
{
    printf("Y: ");
    struct Matrix *Y = get_Y();
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, 1, 0, 1, 0);
    expected = matrix_set_complex(expected, -1, 0, 0, 1);
    
    bool res = matrix_equal(Y, expected, 1e-10);

    free_matrix(Y);
    free_matrix(expected);
    return res;
}

bool test_gate_Z(void)
{
    printf("Z: ");
    struct Matrix *Z = get_Z();
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, 1, 0, 0, 0);
    expected = matrix_set_complex(expected, -1, 0, 1, 1);
    
    bool res = matrix_equal(Z, expected, 1e-10);

    free_matrix(Z);
    free_matrix(expected);
    return res;
}

/*
* CX gate: control = 0, target = 1
*/
bool test_gate_CX_1(void)
{
    printf("CX_1: ");
    size_t controls[] = { 0 };
    struct List *ctrl = size_array_to_list(controls, 1);
    struct Matrix *CX = get_control_U(X, 0, ctrl, 1);
    list_free(ctrl, free);
    
    struct Matrix *expected = init_matrix(4, 4);
    expected = matrix_set_complex(expected, 1, 0, 0, 0);
    expected = matrix_set_complex(expected, 1, 0, 1, 1);
    expected = matrix_set_complex(expected, 1, 0, 2, 3);
    expected = matrix_set_complex(expected, 1, 0, 3, 2);

    bool res = matrix_equal(CX, expected, 1e-10);

    free_matrix(CX);
    free_matrix(expected);
    return res;
}

/*
* CX gate: control = 1, target = 0
*/
bool test_gate_CX_2(void)
{
    printf("CX_2: ");
    size_t controls[] = { 1 };
    struct List *ctrl = size_array_to_list(controls, 1);
    struct Matrix *CX = get_control_U(X, 0, ctrl, 0);
    list_free(ctrl, free);

    struct Matrix *expected = init_matrix(4, 4);
    expected = matrix_set_complex(expected, 1, 0, 0, 0);
    expected = matrix_set_complex(expected, 1, 0, 1, 3);
    expected = matrix_set_complex(expected, 1, 0, 2, 2);
    expected = matrix_set_complex(expected, 1, 0, 3, 1);

    bool res = matrix_equal(CX, expected, 1e-10);

    free_matrix(CX);
    free_matrix(expected);
    return res;
}

bool (*gates_test_functions[])(void) = {
    test_gate_Rx,
    test_gate_Ry,
    test_gate_Rz,
    test_gate_X,
    test_gate_H,
    test_gate_Y,
    test_gate_Z,
    test_gate_CX_1,
    test_gate_CX_2,
    NULL
};

void run_tests_gates(void)
{
    printf("========== TESTS GATES ==========\n");
    float success = 0;
    bool (*test)(void) = NULL;
    size_t i = 0;
    while ((test = (*gates_test_functions[i])))
    {
        printf("Test ");
        bool res = test();
        success += res == true ? 1 : 0;
        if (res)
            printf("PASSED\n");
        else
            printf("FAILED\n");
        ++i;
    }

    printf("Gates tests passed = %f%%\n", success / (float) i * 100);
}