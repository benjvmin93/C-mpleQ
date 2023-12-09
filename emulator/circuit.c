#include "circuit.h"
#include "../utils/alloc.h"
#include "../utils/matrix.h"
#include "math.h"

struct Circuit *init_circuit(size_t nb_qubits)
{
    struct Circuit *circuit = xmalloc(sizeof(struct Circuit));
    circuit->nb_qubits = nb_qubits;
    circuit->vector_state = init_matrix(1, pow(2, circuit->nb_qubits));
    circuit->vector_state = matrix_set_complex(circuit->vector_state, 1, 0, 0, 0);
    // TODO : implement circuit_add_gate(circuit, gatename, controls, targets)
    circuit->circuit = NULL;
}

void free_circuit(struct Circuit *circuit)
{
    free_matrix(circuit->vector_state);
    free(circuit);
}