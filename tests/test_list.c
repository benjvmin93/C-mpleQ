#include <assert.h>
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

void test_list_add()
{
    int *a = xmalloc(sizeof(int));
    int *b = xmalloc(sizeof(int));
    int *c = xmalloc(sizeof(int));
    int *d = xmalloc(sizeof(int));
    *a = 1019293123;
    *b = 2;
    *c = 3;
    *d = 4;

    
    print_int(a);
    struct List *list = init_list(sizeof(int));
    list = list_add(list, a);
    printf("caca%d", *(int*)list->data);
    list = list_add(list, b);
    list = list_add(list, c);
    list = list_add(list, d);
    list_print(list, print_int);

    struct List *l1 = list_find(list, a, int_equal_func);
    list_print(l1, print_int);
    struct List *l2 = list_find(list, b, int_equal_func);
    list_print(l2, print_int);
    struct List *l3 = list_find(list, a, int_equal_func);
    list_print(l3, print_int);
    struct List *l4 = list_find(list, a, int_equal_func);
    list_print(l4, print_int);
    
    list_free(list, free_int);
    /*assert(*(int*)l1->data == *a);
    assert(*(int*)l2->data == *b);
    assert(*(int*)l3->data == *c);
    assert(*(int*)l4->data == *d);*/
}