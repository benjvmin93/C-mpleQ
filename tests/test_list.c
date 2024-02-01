#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "../utils/alloc.h"
#include "../utils/list.h"


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

bool test_list_length_1(void)
{
    struct List *l = init_list(sizeof(int));
    size_t len = list_length(l);

    bool res = len == 1;

    list_free(l, free_int);
    return res;
}

bool test_list_remove_1(void)
{
    int *a = xmalloc(sizeof(int));
    int *b = xmalloc(sizeof(int));
    *a = 1019293123;
    *b = 2;

    struct List *list = init_list(sizeof(int));
    list = list_insert(list, a, 0);
    list = list_insert(list, b, 1);

    list = list_remove(list, a, int_equal_func, free_int);

    bool res = (list_length(list) == 1 && list_at(list, 0) == list && list_at(list, 1) == NULL);

    list_free(list, free_int);

    return res;
}

bool test_list_remove_2(void)
{
    int *a = xmalloc(sizeof(int));
    int *b = xmalloc(sizeof(int));
    *a = 1019293123;
    *b = 2;

    struct List *list = init_list(sizeof(int));
    list = list_insert(list, a, 0);
    list = list_insert(list, b, 1);

    list = list_remove(list, b, int_equal_func, free_int);

    bool res = (list_length(list) == 1 && list_at(list, 0) == list && list_at(list, 1) == NULL);

    list_free(list, free_int);

    return res;
}

bool test_list_at(void)
{
    int *a = xmalloc(sizeof(int));
    int *b = xmalloc(sizeof(int));
    *a = 1019293123;
    *b = 2;

    struct List *list = init_list(sizeof(int));
    list = list_insert(list, a, 0);
    list = list_insert(list, b, 0);

    bool res = (int_equal_func(list_at(list, 0)->data, b)) && (int_equal_func(list_at(list, 1)->data, a));

    list_free(list, free_int);

    return res;
}

bool test_list_insert_beginning(void)
{
    int *a = xmalloc(sizeof(int));
    int *b = xmalloc(sizeof(int));
    int *c = xmalloc(sizeof(int));
    int *d = xmalloc(sizeof(int));
    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    struct List *list = init_list(sizeof(int));
    list = list_insert(list, a, 0);
    list = list_insert(list, b, 0);
    list = list_insert(list, c, 0);
    list = list_insert(list, d, 0);

    bool res = (int_equal_func(list_at(list, 0)->data, d));
    res = (int_equal_func(list_at(list, 1)->data, c));
    res = (int_equal_func(list_at(list, 2)->data, b));
    res = (int_equal_func(list_at(list, 3)->data, a));

    list_free(list, free_int);

    return res;
}

bool test_list_insert_end(void)
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
    list = list_insert(list, a, 0);
    list = list_insert(list, b, 0);
    list = list_insert(list, c, 0);
    list = list_insert(list, d, 0);

    size_t len = list_length(list);
    
    int *e = xmalloc(sizeof(int));
    *e = 42;

    list = list_insert(list, e, len);

    bool res = (int_equal_func(list_at(list, len)->data, e));
    list_free(list, free_int);

    return res;
}

bool test_list_insert_multiple(void)
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
    list = list_insert(list, a, 0);
    list = list_insert(list, b, 0);
    list = list_insert(list, c, 0);
    list = list_insert(list, d, 0);

    bool res = (int_equal_func(list_at(list, 3)->data, a));
    res = res && (int_equal_func(list_at(list, 2)->data, b));
    res = res && (int_equal_func(list_at(list, 1)->data, c));
    res = res && (int_equal_func(list_at(list, 0)->data, d));

    list_free(list, free_int);

    return res;
}

bool test_list_insert_middle(void)
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
    list = list_insert(list, a, 0);
    list = list_insert(list, b, 0);
    list = list_insert(list, c, 0);
    list = list_insert(list, d, 0);
    size_t len = list_length(list);
    int *e = xmalloc(sizeof(int));
    *e = 42;

    list = list_insert(list, e, len / 2);
    bool res = (int_equal_func(list_at(list, 2)->data, e));

    list_free(list, free_int);

    return res;
}

bool test_list_reverse_1(void)
{
    int *a = xmalloc(sizeof(int));
    *a = 100;
    struct List *l = init_list(sizeof(int));
    l = list_append(l, a);
    struct List *reversed = list_reverse(l);

    bool res = list_length(reversed) == 1 && *(int*)list_at(reversed, 0)->data == 100;
    list_free(l, free_int);
    list_free(reversed, free_int);
    return res;                                                            
}

bool test_list_reverse_2(void)
{
    int *a = xmalloc(sizeof(int));
    int *b = xmalloc(sizeof(int));
    int *c = xmalloc(sizeof(int));
    int *d = xmalloc(sizeof(int));
    *a = 1;
    *b = 2;
    *c = 3;
    *d = 4;

    struct List *l = init_list(sizeof(int));
    l = list_append(l, a);
    l = list_append(l, b);
    l = list_append(l, c);
    l = list_append(l, d);
    struct List *reversed = list_reverse(l);

    bool res = list_length(reversed) == 4 &&
        *(int*)list_at(reversed, 0)->data == 4 && *(int*)list_at(reversed, 1)->data == 3 &&
        *(int*)list_at(reversed, 2)->data == 2 && *(int*)list_at(reversed, 3)->data == 1;
    
    list_free(l, free_int);
    list_free(reversed, free_int);
    return res;                                                            
}

bool (*lists_test_functions[])(void) = {
    test_list_length_1,
    test_list_remove_1,
    test_list_remove_2,
    test_list_at,
    test_list_insert_beginning,
    test_list_insert_end,
    test_list_insert_multiple,
    test_list_insert_middle,
    test_list_reverse_1,
    test_list_reverse_2,
    NULL
};

void run_tests_lists(void)
{
    float success = 0;
    bool (*test)(void) = NULL;
    size_t i = 0;
    while ((test = (*lists_test_functions[i])))
    {
        printf("Test %ld: ", i++ + 1);
        bool res = test();
        success += res == true ? 1 : 0;
        if (res)
            printf("PASSED\n");
        else
            printf("FAILED\n");
    }

    printf("List tests passed = %f%%\n", success / (float)i * 100);
}