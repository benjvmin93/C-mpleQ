#include <math.h>
#include <stdio.h>

#include "../utils/pi.h"
#include "../emulator/gates.h"

#define NB_TESTS_GATES 3

double PI;

bool test_gate_Rx(void)
{
    double theta = PI;
    struct Gate *Rx = init_gate(RX, theta, NULL, NULL);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, cos(theta / 2), 0, 0, 0);
    expected = matrix_set_complex(expected, cos(theta / 2), 0, 1, 1);
    expected = matrix_set_complex(expected, 0, -sin(theta / 2), 1, 0);
    expected = matrix_set_complex(expected, 0, -sin(theta / 2), 0, 1);

    bool res = matrix_equal(Rx->m_gate, expected);

    free_gate(Rx);
    free_matrix(expected);
    return res;
}

bool test_gate_Ry(void)
{
    double theta = PI;
    struct Gate *Ry = init_gate(RY, theta, NULL, NULL);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, cos(theta / 2), 0, 0, 0);
    expected = matrix_set_complex(expected, cos(theta / 2), 0, 1, 1);
    expected = matrix_set_complex(expected, -sin(theta / 2), 0, 1, 0);
    expected = matrix_set_complex(expected, sin(theta / 2), 0, 0, 1);

    bool res = matrix_equal(Ry->m_gate, expected);

    free_gate(Ry);
    free_matrix(expected);
    return res;
}

bool test_gate_Rz(void)
{
    double theta = PI;
    struct Gate *Rz = init_gate(RZ, theta, NULL, NULL);
    struct Matrix *expected = init_matrix(2, 2);
    expected = matrix_set_complex(expected, cos(theta / 2), -sin(theta / 2), 0, 0);
    expected = matrix_set_complex(expected, cos(theta / 2), sin(theta / 2), 1, 1);

    bool res = matrix_equal(Rz->m_gate, expected);

    free_gate(Rz);
    free_matrix(expected);
    return res;
}

bool (*gates_test_functions[])(void) = {
    test_gate_Rx,
    test_gate_Ry,
    test_gate_Rz
};

void run_tests_gates(void)
{
    PI = calculate_PI(100000);
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