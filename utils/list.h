#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdlib.h>

struct List
{
    size_t data_size;
    void *data;
    struct List *next;
};

struct List *init_list(size_t data_size);
struct List *list_insert(struct List *l, void *data, size_t index);
struct List *list_at(struct List *l, size_t index);
size_t list_length(struct List *l);
void list_print(struct List *l, void (*print_fun)(void *));
struct List *list_find(struct List *l, void *data, bool (*equal_fun)(void *, void *));
struct List *list_remove(struct List *l, void *data, bool (*equal_fun)(void *, void *), void (*free_fun)(void *));
struct List *list_remove_if(struct List *l, int (*predicate)(void *), bool (*equal_fun)(void *, void *), void (*free_fun)(void *));
void list_free(struct List *l, void (*free_fun)(void *));

#endif