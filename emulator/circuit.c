#define _XOPEN_SOURCE 500

#include <string.h>
#include <stdio.h>

#include "circuit.h"
#include "gates.h"
#include "../utils/alloc.h"
#include "../utils/matrix.h"
#include "math.h"

struct Circuit *init_circuit(size_t nb_qubits)
{
    struct Circuit *circuit = xmalloc(sizeof(struct Circuit));
    circuit->nb_qubits = nb_qubits;
    circuit->vector_state = init_matrix(1, pow(2, circuit->nb_qubits));
    circuit->vector_state = matrix_set_complex(circuit->vector_state, 1, 0, 0, 0);
    circuit->gates = NULL;
    circuit->measurement = NULL;
    printf("Initialized a circuit of %ld qubits\n", nb_qubits);
    return circuit;
}

void free_circuit(struct Circuit *circuit)
{
    free_matrix(circuit->vector_state);
    list_free(circuit->gates, free_gate);
    list_free(circuit->measurement, free);
    free(circuit);
}

bool size_t_equal_func(void *a, void *b)
{
    size_t *s1 = a;
    size_t *s2 = b;
    return *s1 == *s2;
}

struct Circuit *circuit_add_gate(struct Circuit *circuit, int gate, double rotation, struct List *controls, struct List *targets)
{
    if (circuit->gates == NULL)
    {
        circuit->gates = init_list(sizeof(struct Gate*));
    }

    for (struct List *t = targets; t; t = t->next)
    {   
        size_t *target = t->data;
        struct Gate *new_gate = init_gate(gate, rotation, controls, *target);
        circuit->gates = list_append(circuit->gates, new_gate);
    }
    list_free(targets, free);
    return circuit;
}

struct Circuit *circuit_measure(struct Circuit *circuit, struct List *targets)
{   
    circuit->measurement = init_list(sizeof(size_t));
    for (struct List *t = targets; t; t = t->next)
    {   
        size_t *target = t->data;
        struct Gate *new_gate = init_gate(M0, 0, NULL, *target);
        circuit->measurement = list_append(circuit->measurement, new_gate);
    }
    return circuit;
}

void print_circuit(struct Circuit *circuit)
{
    struct List *gates = circuit->gates;
    struct List *lines = init_list(sizeof(char *));
    char *initial_str = "";
    for (size_t i = 0; i < circuit->nb_qubits; ++i)
    {
        list_append(lines, strdup(initial_str));
    }

    char *qubit = "|0>";
    char *sep = "--";
    for (struct List *l = lines; l != NULL; l = l->next)
    {
        l->data = realloc(l->data, strlen(l->data) + 1 + strlen(qubit) + 1);
        l->data = strcat(l->data, qubit);
    }

    size_t max_line_length = 0;
    for (struct List *g = gates; g->next; g = g->next)
    {
        struct Gate *gate = g->data;
        size_t target = gate->target;
        struct List *line = list_at(lines, target);
        size_t new_length = strlen(line->data) + 1 + strlen(sep) + 1 + strlen(gate->name) + 1;
        max_line_length = max_line_length < new_length ? new_length : max_line_length;
        line->data = realloc(line->data, new_length);
        line->data = strcat(line->data, sep);
        line->data = strcat(line->data, gate->name);
    }
    for (struct List *l = lines; l != NULL; l = l->next)
    {
        char *str = l->data;
        while (strlen(str) < max_line_length)
        {
            str = realloc(str, strlen(str) + 1 + strlen("-") + 1);
            str = strcat(str, "-");
        }
        printf("%s\n", str);
    }
    list_free(lines, free);
}