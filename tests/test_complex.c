#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "../utils/complex.h"
#include "tests.h"

#define NB_TESTS_COMPLEX 25

long double TEST_VALUES[NB_TESTS_COMPLEX][4] = {
    { 2.79, -9.5, -4.5, -5.54 },
    { 4.73, 3.53, 7.84, -8.26 },
    { -1.56, -9.4, -5.63, 0.11 },
    { -9.47, -6.02, 3.0, 0.9 },
    { -5.59, 1.79, 6.19, -9.87 },
    { 6.12, 3.96, -3.19, -6.89 },
    { 9.14, -3.27, -8.15, -8.07 },
    { 6.95, 2.07, 6.14, 4.59 },
    { 0.72, 9.46, -2.43, 1.04 },
    { 6.59, 2.37, 7.23, 1.55 },
    { 4.09, -9.08, -5.44, -4.21 },
    { -8.4, -5.34, -7.98, -4.44 },
    { 2.71, -2.7, -2.6, -5.81 },
    { -4.66, 8.73, 2.96, 2.18 },
    { -6.58, 4.58, -6.73, -2.41 },
    { 9.79, 2.8, 1.14, 3.69 },
    { 6.86, 5.52, -5.42, -9.36 },
    { -3.69, -4.65, -5.78, 8.86 },
    { 7.53, -3.71, 3.11, -2.09 },
    { 8.29, -0.82, -4.7, -5.07 },
    { 1.23, -4.75, 1.69, 7.96 },
    { -2.01, -5.61, 9.95, 0.19 },
    { -8.18, -9.06, -7.81, 2.55 },
    { 5.84, -1.56, -8.73, -2.37 },
    { 9.92, 0.58, 9.42, 7.22 }
};

long double EXPECTED_RESULTS[NB_TESTS_COMPLEX][2] = {
    { -1.71, -15.04 },
    { 12.57, -4.73},
    { -7.19, -9.29 },
    { -6.47, -5.12 },
    { 0.6, -8.08 },
    { 2.93, -2.93 },
    { 1.0, -11.33 },
    { 13.09, 6.67 },
    { -1.71, 10.5 },
    { 13.82, 3.92 },
    { -1.35, -13.29 },
    { -16.38, -9.79 },
    { 0.11, -8.51 },
    { -1.7, 10.92 },
    { -13.31, 2.17 },
    { 10.93, 6.49 },
    { 1.44, -3.84 },
    { -9.47, 4.21 },
    { 10.64, -5.79 },
    { 3.59, -5.89 },
    { 2.92, 3.21 },
    { 7.94, -5.42 },
    { -15.99, -6.51 },
    { -2.89, -3.93 },
    { 19.34, 7.8 }
};

void run_tests_complex(void)
{
    printf("========== TESTS COMPLEX ==========\n");
    size_t success = 0;

    for (size_t i = 0; i < NB_TESTS_COMPLEX; ++i)
    {
        printf("Test %ld: ", i);
        double a = TEST_VALUES[i][0];
        double b = TEST_VALUES[i][1];
        double c = TEST_VALUES[i][2];
        double d = TEST_VALUES[i][3];
        double expected_a = EXPECTED_RESULTS[i][0];
        double expected_b = EXPECTED_RESULTS[i][1];

        struct Complex *z1 = init_complex(a, b);
        struct Complex *z2 = init_complex(c, d);

        struct Complex *expected = init_complex(expected_a, expected_b);
        struct Complex *result = complex_add(z1, z2);
        bool test_passed = complex_equal(expected, result, 0.01);

        if (!test_passed)
        {
            printf("FAILED\n");
            print_complex(result);
            print_complex(expected);
        }
        else
        {
            printf("PASSED\n");
            success++;
        }
        free_complex(z1);
        free_complex(z2);
        free_complex(expected);
        free_complex(result);
}

    printf("Complex tests passed = %f%%\n", (success / (double)NB_TESTS_COMPLEX) * 100);
}