#include <math.h>
#include <stdio.h>

#include "../utils/alloc.h"
#include "emulator.h"
#include "gates.h"

void print_matrix_void(void *m)
{
    struct Matrix *mm = (struct Matrix *)m;
    print_matrix(mm);
}

/*
struct Result *init_result(struct Matrix *vector_state)
{
    return NULL;
}

void free_result(struct Result *result)
{
    return;
}
*/

/*
* Function: get_system_unitary
* ----------------
* 
* circuit: circuit structure which we will get the unitary matrix
*
* returns: the whole system unitary matrix
*/
struct Matrix *get_system_unitary(struct Circuit *circuit)
{
    struct List *gate = circuit->gates;
    struct List *unitary_gates = init_list(sizeof(struct Matrix));
    while (gate)
    {
        struct Gate *g = gate->data;
        struct Matrix *unitary_gate = build_unitary_gate(g, circuit->nb_qubits);
        unitary_gates = list_append(unitary_gates, unitary_gate);
        gate = gate->next;
    }

    struct List *unitary_reversed = list_reverse(unitary_gates);
    list_free(unitary_gates, free_matrix);
    size_t mat_size = pow(2, circuit->nb_qubits);
    struct Matrix *system_unitary = identity(mat_size);

    for (size_t i = 0; i < list_length(unitary_reversed); i++)
    {
        struct Matrix *old_system_unitary = system_unitary;
        struct Matrix *m = list_at(unitary_reversed, i)->data;
        system_unitary = matrix_mul(old_system_unitary, m);
        free_matrix(old_system_unitary);
    }
    list_free(unitary_reversed, free_matrix);
    return system_unitary;
}

struct Emulator *init_emulator(void)
{
    struct Emulator *emulator = xmalloc(sizeof(struct Emulator));
    emulator->emulate_func = run_emulation;
    return emulator;
}
void free_emulator(struct Emulator *emulator)
{
    free(emulator);
}

struct Result *run_emulation(struct Circuit *circuit, size_t shots)
{
    // Get the whole system unitary
    struct Matrix *system_unitary = get_system_unitary(circuit);
    
    // Multiply the system unitary with the state vector
    struct Matrix *state = circuit->vector_state;
    struct Matrix *transpose_state = matrix_transpose(state);
    struct Matrix *result = matrix_mul(transpose_state, system_unitary);
    free_matrix(transpose_state);
    free_matrix(system_unitary);
    if (list_length(circuit->measurement) >= 1 && circuit->measurement->data != NULL)
    {
        // Perform measurement
        struct Gate *M = init_gate(M0, 0, NULL, circuit->measurement);
        struct Matrix *measurement_unitary = build_unitary_gate(M, circuit->nb_qubits);
        print_matrix(result);
        print_matrix(measurement_unitary);
    }

    free_matrix(result);
    return NULL;
    
}
