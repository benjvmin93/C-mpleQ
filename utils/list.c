#include <stdio.h>

#include "alloc.h"
#include "list.h"

struct List *init_list(size_t data_size)
{
    struct List *l = xmalloc(sizeof(struct List));
    l->data_size = data_size;
    l->data = NULL;
    l->next = NULL;
    return l;
}

struct List *list_add(struct List *l, void *data)
{
    struct List *new = init_list(l->data_size);
    new->data = data;
    new->next = l;

    return new;
}

void list_print(struct List *l, void (*print_fun)(void *))
{
    if (!l)
    {
        printf("\n");
        return;
    }
    while (l->next)
    {
        print_fun(l->data);
        l = l->next;
    }
    printf("\n");
}

struct List *list_find(struct List *l, void *data, bool (*equal_fun)(void *, void *))
{
    if (!l)
        return NULL;

    while (l->next)
    {
        if (equal_fun(l->data, data))
            return l;
        l = l->next;
    }
    if (equal_fun(l->data, data))
        return l;
    return NULL;
}

struct List *list_remove(struct List *l, void *data, bool (*equal_fun)(void *, void *), void (*free_fun)(void *))
{
    if (!l)
        return NULL;

    struct List *head = l;

    if (equal_fun(l->data, data))
    {
        l = l->next;
        list_free(head, free_fun);
        return l;
    }

    while (l->next && !equal_fun(l->next->data, data))
        l = l->next;

    if (!l->next)
    {
        if (equal_fun(l->data, data))
            list_free(l, free_fun);
        return head;
    }

    struct List *tmp = l->next;
    l->next = l->next->next;
    list_free(tmp, free_fun);

    return head;
}

struct List *list_remove_if(struct List *l, int (*predicate)(void *), bool (*equal_fun)(void *, void *), void (*free_fun)(void *))
{
    if (!l)
        return l;
    struct List *head = l;
    while (head)
    {
        if (predicate(head->data))
        {
            l = list_remove(l, head->data, equal_fun, free_fun);
            head = l;
        }
        else
            head = head->next;
    }

    return l;
}

void list_free(struct List *l, void (*free_fun)(void *))
{
    while (l)
    {
        struct List *tmp = l;
        free_fun(l->data);
        l = l->next;
        free(tmp);
    }
}