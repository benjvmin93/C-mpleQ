#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "../utils/alloc.h"
#include "../utils/list.h"

#define NB_TESTS_LISTS 1

bool int_equal_func(void *a, void *b)
{
    return *(int*)a == *(int*)b;
}

void print_int(void *a)
{
    printf("%d ", *(int*)a);
}

void free_int(void *a)
{
    free(a);
}

bool test_list_add(void)
{
    int *a = xmalloc(sizeof(int));
    int *b = xmalloc(sizeof(int));
    int *c = xmalloc(sizeof(int));
    int *d = xmalloc(sizeof(int));
    *a = 1019293123;
    *b = 2;
    *c = 3;
    *d = 4;

    
    struct List *list = init_list(sizeof(int));
    list = list_add(list, a);
    list = list_add(list, b);
    list = list_add(list, c);
    list = list_add(list, d);

    struct List *l1 = list_find(list, a, int_equal_func);
    struct List *l2 = list_find(list, b, int_equal_func);
    struct List *l3 = list_find(list, c, int_equal_func);
    struct List *l4 = list_find(list, d, int_equal_func);
    
    bool res = ((*(int*)l1->data == *a) && (*(int*)l2->data == *b) && (*(int*)l3->data == *c) && (*(int*)l4->data == *d));
    list_free(list, free_int);

    return res;
}

bool (*lists_test_functions[])(void) = {
    test_list_add,
};

void run_tests_lists(void)
{
    printf("========== TESTS LIST ==========\n");
    float success = 0;
    for (size_t i = 0; i < NB_TESTS_LISTS; ++i)
    {
        printf("Test %ld: ", i + 1);
        bool res = (*lists_test_functions[i])();
        success += res == true ? 1 : 0;
        if (res)
            printf("PASSED\n");
        else
            printf("FAILED\n");
    }

    printf("List tests passed = %f%%\n", success / (float)NB_TESTS_LISTS * 100);
}