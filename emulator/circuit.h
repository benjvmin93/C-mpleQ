#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <stdlib.h>

#include "../utils/list.h"

struct Circuit {
    size_t nb_qubits;
    struct Matrix *vector_state;
    struct List *gates;
};

struct Circuit *init_circuit(size_t nb_qubits);
void free_circuit(struct Circuit *circuit);
struct Circuit *circuit_add_gate(struct Circuit *circuit, enum Gates gate, struct List *controls, struct List *targets);

#endif