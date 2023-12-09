#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <stdlib.h>

struct Circuit {
    size_t nb_qubits;
    struct Matrix *vector_state;
    struct Matrix **circuit;
};

struct Circuit *init_circuit(size_t nb_qubits);
void free_circuit(struct Circuit *circuit);

#endif