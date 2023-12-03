#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../utils/complex.h"
#include "tests.h"

#define NB_TESTS 5

/*               {-3.691, -4.645, -5.78, 8.858}, {7.527, -3.706, 3.109, -2.087}, {8.291, -0.823, -4.702, -5.067}, {1.227, -4.745, 1.692, 7.956}, {-2.012, -5.614, 9.951, 0.191}, {-8.182, -9.058, -7.807, 2.549}, {5.842, -1.557, -8.729, -2.368}, {9.922, 0.582, 9.422, 7.216}};
*/
long double TEST_VALUES[NB_TESTS][4] = {
    { 2.79, -9.5, -4.5, -5.54 },
    { 4.73, 3.53, 7.84, -8.26 },
    { -1.56, -9.4, -5.63, 0.11 },
    { -9.47, -6.02, 3.0, 0.9 },
    { -5.59, 1.79, 6.19, -9.87 },
    /*{ 6.12, 3.96, -3.19, -6.89 },
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
    { 9.92, 0.58, 9.42, 7.22 }*/
};

long double EXPECTED_RESULTS[NB_TESTS][2] = {
    { -1.71, -15.04 },
    { 12.57, -4.73},
    { -7.19, -9.29 },
    { -6.47, -5.12 },
    { 0.6, -8.08 },
    /*{ 2.92, -2.93 },
    { 1.0, -11.33 },
    { 13.09, 6.67 },
    { -1.7, 10.5 },
    { 13.82, 3.92 },
    { -1.35, -13.3 },
    { -16.38, -9.79 },
    { 0.12, -8.51 },
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
    { -2.89, -3.92 },
    { 19.34, 7.8 }*/
};

/*
,    , {2.921, -2.9269999999999996}, {0.9990000000000006, -11.334}, {13.093, 6.67}, {-1.7039999999999997, 10.503}, {13.822, 3.917}, {-1.351, -13.296}, {-16.384, -9.785}, {0.11799999999999988, -8.513}, {-1.6990000000000003, 10.916}, {-13.309000000000001, 2.172}, {10.928999999999998, 6.492}, {1.4380000000000006, -3.838000000000001}, {-9.471, 4.213000000000001}, {10.636, -5.793}, {3.5890000000000004, -5.890000000000001}, {2.919, 3.2110000000000003}, {7.939, -5.423}, {-15.989, -6.509}, {-2.8869999999999996, -3.925}, {19.344, 7.798}};
*/
void test_complex(void)
{
    size_t fail = 0;

    for (size_t i = 0; i < NB_TESTS; ++i)
    {
        printf("%ld\n", i);
        struct Complex *z1 = init_complex(TEST_VALUES[i][0], TEST_VALUES[i][1]);
        struct Complex *z2 = init_complex(TEST_VALUES[i][2], TEST_VALUES[i][3]);
        struct Complex *expected = init_complex(EXPECTED_RESULTS[i][0], EXPECTED_RESULTS[i][1]);

        struct Complex *result = complex_add(z1, z2);

        bool test_passed = (*result->a == *expected->a) && (*result->b == *expected->b);
        if (!test_passed)
        {
            printf("res_a = %f\n", *result->a);
            printf("res_b = %f\n", *result->b);
            printf("exp_a = %f\n", *expected->a);
            printf("exp_b = %f\n", *expected->b);
            fail++;
            print_complex(result);
            print_complex(expected);
        }

        free_complex(z1);
        free_complex(z2);
        free_complex(expected);
        free_complex(result);

}

    printf("Fail rate = %ld\n", fail / NB_TESTS);
}