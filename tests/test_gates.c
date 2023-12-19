#include <stdio.h>

#include "../utils/maths.h"
#include "../emulator/gates.h"

#define NB_TESTS_GATES 5

bool test_gate_Rx(void)
{
    double theta = M_PI;
    struct Gate *Rx = init_gate(RX, theta, NULL, NULL);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 0, 0);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 1, 1);
    expected = matrix_set_complex(expected, 0, -msin(theta / 2), 1, 0);
    expected = matrix_set_complex(expected, 0, -msin(theta / 2), 0, 1);

    bool res = matrix_equal(Rx->m_gate, expected);

    free_gate(Rx);
    free_matrix(expected);
    return res;
}

bool test_gate_Ry(void)
{
    double theta = M_PI;
    struct Gate *Ry = init_gate(RY, theta, NULL, NULL);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 0, 0);
    expected = matrix_set_complex(expected, mcos(theta / 2), 0, 1, 1);
    expected = matrix_set_complex(expected, -msin(theta / 2), 0, 1, 0);
    expected = matrix_set_complex(expected, msin(theta / 2), 0, 0, 1);

    bool res = matrix_equal(Ry->m_gate, expected);

    free_gate(Ry);
    free_matrix(expected);
    return res;
}

bool test_gate_Rz(void)
{
    double theta = M_PI;
    struct Gate *Rz = init_gate(RZ, theta, NULL, NULL);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, mcos(theta / 2), -msin(theta / 2), 0, 0);
    expected = matrix_set_complex(expected, mcos(theta / 2), msin(theta / 2), 1, 1);

    bool res = matrix_equal(Rz->m_gate, expected);

    free_gate(Rz);
    free_matrix(expected);
    return res;
}

bool test_gate_X(void)
{
    struct Matrix *X = get_X();
    struct Matrix *expected = init_matrix(2, 2);
    
    expected = matrix_set_complex(expected, 0, -1, 1, 0);
    expected = matrix_set_complex(expected, 0, -1, 0, 1);
    printf("\n");
    print_matrix(X);
    printf("\n");
    print_matrix(expected);
    
    bool res = matrix_equal(X, expected);

    free_matrix(X);
    free_matrix(expected);
    return res;
}

bool test_gate_H(void)
{
    struct Matrix *H = get_H();
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, 0, -1, 1, 0);
    expected = matrix_set_complex(expected, 0, -1, 0, 1);
    printf("\n");
    print_matrix(H);
    print_matrix(expected);
    
    bool res = matrix_equal(H, expected);

    free_matrix(H);
    free_matrix(expected);
    return res;
}

bool (*gates_test_functions[])(void) = {
    test_gate_Rx,
    test_gate_Ry,
    test_gate_Rz,
    test_gate_X,
    test_gate_H
};

void run_tests_gates(void)
{
    printf("========== TESTS GATES ==========\n");
    float success = 0;
    for (size_t i = 0; i < NB_TESTS_GATES; ++i)
    {
        printf("Test %ld: ", i + 1);
        bool res = (*gates_test_functions[i])();
        success += res == true ? 1 : 0;
        if (res)
            printf("PASSED\n");
        else
            printf("FAILED\n");
    }

    printf("Gates tests passed = %f%%\n", success / (float)NB_TESTS_GATES * 100);
}