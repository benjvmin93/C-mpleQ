#include <stdbool.h>
#include <stdio.h>

#include "../utils/maths.h"

bool test_maths_cos_1(void)
{
    double cos_0 = mcos(0);
    return cos_0 == 1;
}
bool test_maths_cos_2(void)
{
    double cos_0 = mcos(M_PI);
    return cos_0 == -1;
}
bool test_maths_cos_3(void)
{
    double cos_0 = mcos(5 * M_PI);
    return cos_0 == -1;
}
bool test_maths_cos_4(void)
{
    double cos_0 = mcos(2 * M_PI);
    return cos_0 == 1;
}
bool test_maths_cos_5(void)
{
    double cos_0 = mcos(10 * M_PI);
    return cos_0 == 1;
}
bool test_maths_cos_6(void)
{
    double cos_0 = mcos(21 * M_PI);
    return cos_0 == -1;
}
bool test_maths_cos_7(void)
{
    double cos_0 = mcos(M_PI / 4);
    return cos_0 - 0.70710678118 < 1e-10;
}
bool test_maths_sin_8(void)
{
    double cos_0 = msin(M_PI / 2);
    return cos_0 == 1.0;
}
bool test_maths_sin_9(void)
{
    double cos_0 = msin(3 * M_PI / 2);
    return cos_0 == -1.0;
}
bool test_maths_sin_10(void)
{
    double cos_0 = msin(M_PI);
    return cos_0 == 0.0;
}
bool test_maths_sin_11(void)
{
    double cos_0 = msin(2 * M_PI);
    return cos_0 == 0.0;
}
bool test_maths_sin_12(void)
{
    double cos_0 = msin(5 * M_PI / 2);
    return cos_0 == 1.0;
}
bool test_maths_sin_13(void)
{
    double cos_0 = msin(7 * M_PI / 2);
    return cos_0 == -1.0;
}

bool (*maths_test_functions[])(void) = {
    test_maths_cos_1,
    test_maths_cos_2,
    test_maths_cos_3,
    test_maths_cos_4,
    test_maths_cos_5,
    test_maths_cos_6,
    test_maths_cos_7,
    test_maths_sin_8,
    test_maths_sin_9,
    test_maths_sin_10,
    test_maths_sin_11,
    test_maths_sin_12,
    test_maths_sin_13,
    NULL
};

void run_tests_maths(void)
{
    printf("========== TESTS MATHS ==========\n");
    float success = 0;
    bool (*test)(void) = NULL;
    size_t i = 0;
    while ((test = (*maths_test_functions[i])))
    {
        printf("Test %ld: ", i++ + 1);
        bool res = test();
        success += res == true ? 1 : 0;
        if (res)
            printf("PASSED\n");
        else
            printf("FAILED\n");
    }

    printf("Maths tests passed = %f%%\n", success / (float)i * 100);
}