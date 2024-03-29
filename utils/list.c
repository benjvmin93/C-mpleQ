#include <stdio.h>
#include <err.h>

#include "alloc.h"
#include "list.h"

/*
 * Function: init_list
 * ------------------
 * initialize a new empty list
 * 
 * data_size: size of the data in the list
 * 
 * returns: the list structure
 */
struct List *init_list(size_t data_size)
{
    struct List *l = xmalloc(sizeof(struct List));
    l->data_size = data_size;
    l->data = NULL;
    l->next = NULL;
    return l;
}

/*
 * Function: size_array_to_list
 * ------------------
 * initialize a new list of size_t with the given array
 * 
 * array: array of size_t
 * 
 * returns: the list structure according to the given array
 */
struct List *size_array_to_list(size_t *array, size_t length)
{
    struct List *list = init_list(sizeof(size_t));
    for (size_t i = 0; i < length; ++i)
    {
        size_t *data = xmalloc(sizeof(size_t));
        *data = array[i];
        list = list_append(list, data);
    }

    return list;
}

/*
 * Function: list_at
 * ------------------
 * get the list structure at a specific position.
 * calling list_at(l, i) is equivalent to write l[i]
 * 
 * l: list
 * index: position to get
 * 
 * returns: the list structure at the specific position index
 */
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

struct List *list_reverse(struct List *l)
{
    size_t len = list_length(l);
    struct List *reversed = init_list(l->data_size);
    for (int i = len - 1; i >= 0; i--)
    {
        size_t index = i;
        struct List *elt = list_at(l, index);
        reversed = list_append(reversed, elt->data);
        elt->data = NULL;
    }
    return reversed;
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
        if (l->data)
        {
            free_fun(l->data);
        }
        l = l->next;
        free(tmp);
    }
}

struct List *list_append(struct List *l, void *data)
{
    if (list_length(l) == 1 && l->data == NULL)
    {
        l->data = data;
        return l;
    }

    struct List *new = init_list(l->data_size);
    new->data = data;
    struct List *head = l;
    while (l->next)
    {
        l = l->next;
    }
    l->next = new;
    new->next = NULL;
    return head;
}

struct List *size_list_merge(struct List *l1, struct List *l2)
{
    if (l1 == NULL)
    {
        return l2;
    }
    if (l2 == NULL)
    {
        return l1;
    }

    struct List *res = NULL;
    size_t *s1 = l1->data;
    size_t *s2 = l2->data;
    if (*s1 <= *s2)
    {
        res = l1;
        res->next = size_list_merge(l1->next, l2);
    }
    else
    {
        res = l2;
        res->next = size_list_merge(l1, l2->next);
    }
    return res;
}

struct List *mergeSort(struct List *head)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    struct List *slow = head;
    struct List *fast = head->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct List *secondHalf = slow->next;
    slow->next = NULL;

    return size_list_merge(mergeSort(head), mergeSort(secondHalf));
}

bool size_list_contains(struct List *l, size_t a)
{
    if (l == NULL)
    {
        return false;
    }

    struct List *tmp = l;
    while (tmp)
    {
        size_t *d = tmp->data;
        if (*d == a)
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

void size_list_print(struct List *l)
{
    if (l == NULL)
    {
        return;
    }
    struct List *tmp = l;
    while (tmp)
    {
        size_t *d = tmp->data;
        printf("%ld ", *d);
        tmp = tmp->next;
    }
    printf("\n");
}