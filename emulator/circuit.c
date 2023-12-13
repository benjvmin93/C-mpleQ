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
    circuit->gates = init_list(sizeof(struct Gate));
}

void free_circuit(struct Circuit *circuit)
{
    free_matrix(circuit->vector_state);
    free(circuit);
}

struct Circuit *circuit_add_gate(struct Circuit *circuit, enum Gates gate, struct List *controls, struct List *targets)
{
    struct Gate *new_gate = init_gate(gate, controls, targets);
    circuit->gates = list_append(circuit->gates, new_gate);
    return circuit;
}