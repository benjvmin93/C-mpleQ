#include <stdio.h>

#include "tests.h"

int main(void)
{
    run_tests_maths();
    run_tests_complex();
    run_tests_matrix();
    run_tests_lists();
    run_tests_gates();
    return 0;
}