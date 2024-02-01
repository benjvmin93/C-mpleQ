#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <stdlib.h>

#include "../utils/list.h"

struct Circuit {
    size_t nb_qubits;
    struct Matrix *vector_state;
    struct List *gates;
    struct List *measurement;
};

struct Circuit *init_circuit(size_t nb_qubits);
void free_circuit(struct Circuit *circuit);
struct Circuit *circuit_add_gate(struct Circuit *circuit, int gate, double rotation, struct List *controls, struct List *targets);
struct Circuit *circuit_measure(struct Circuit *circuit, struct List *targets);

#endif