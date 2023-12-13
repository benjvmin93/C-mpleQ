#include <stdio.h>
#include <err.h>

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

struct List *list_at(struct List *l, size_t index)
{
    size_t length = list_length(l);
    if (index > length)
        err(1, "Invalid index range");
    
    struct List *tmp = l;
    while (tmp && index-- > 0)
    {
        tmp = tmp->next;
    }

    return tmp;
}

struct List *list_insert(struct List *l, void *data, size_t index)
{   
    size_t len = list_length(l);
    if (index > len)
    {
        err(1, "Index out of list length bounds");
    }
    if (len == 1 && index == 0 && l->data == NULL)
    {
        l->data = data;
        return l;
    }
    struct List *new = init_list(l->data_size);
    new->data = data;

    if (index == 0)
    {
        new->next = l;
        return new;
    }

    struct List *current = l;
    size_t i = 0;
    while (current != NULL && i < index - 1)
    {
        current = current->next;
        i++;
    }
    new->next = current->next;
    current->next = new;

    return l;
}

size_t list_length(struct List *l)
{
    size_t length = 0;
    struct List *tmp = l;
    while (tmp)
    {
        length++;
        tmp = tmp->next;
    }
    return length;
}

void list_print(struct List *l, void (*print_fun)(void *))
{
    if (!l)
    {
        printf("\n");
        return;
    }
    while (l)
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
    // To remove an element, we need the pointer before and after the one holding the data.
    if (!l)
        return NULL;

    struct List *head = l;

    // Check if the head is holding the data
    if (equal_fun(head->data, data))
    {
        l = head->next;
        head->next = NULL;
        list_free(head, free_fun);
        return l;
    }

    // Otherwise, iterate until we reach end of list or find the data
    while (l->next && !equal_fun(l->next->data, data))
        l = l->next;

    // The data is not in the list.
    if (!l->next)
    {
        return head;
    }

    // Get the element to remove which is the next element.
    struct List *tmp = l->next;
    // Connect new next element to the next of the element to remove.
    l->next = tmp->next;
    // Free element to remove.
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