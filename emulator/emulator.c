#include <math.h>
#include <stdio.h>
#include <time.h>

#include "../utils/alloc.h"
#include "../utils/complex.h"
#include "../utils/maths.h"
#include "emulator.h"
#include "gates.h"

void print_matrix_void(void *m)
{
    struct Matrix *mm = (struct Matrix *)m;
    print_matrix(mm);
}

struct Result *init_result(double p0, double p1, size_t measure_index, const int bit)
{
    struct Result *result = xmalloc(sizeof(struct Result));
    result->p0 = p0;
    result->p1 = p1;
    result->measure_index = measure_index;
    result->bit = bit;
    return result;
}

void free_result(void *result)
{
    struct Result *res = result;
    free(res);
}

void free_results(struct List *results)
{
    list_free(results, free_result);
}

void print_int(void *data)
{
    int *x = data;
    printf("%d", *x);
}

void print_results(struct List *results)
{
    struct List *measured_bits = init_list(sizeof(int));
    struct List *tmp = results;
    while (tmp)
    {
        struct Result *res_i = tmp->data;
        int *bit = xmalloc(sizeof(int));
        *bit = res_i->bit;
        measured_bits = list_append(measured_bits, bit);
        printf("{\n\t0: %f\n\t1: %f\n\tbit: %d\n\ti: %ld\n}\n", res_i->p0, res_i->p1, res_i->bit, res_i->measure_index);
        tmp = tmp->next;
    }
    printf("Measured state: ");
    list_print(measured_bits, print_int);
    list_free(measured_bits, free);
}

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
    size_t mat_size = pow(2, circuit->nb_qubits);
    if (gate == NULL)
    {
        return identity(mat_size);
    }
    
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


/*
 * Function:  get_measurement_unitary
 * --------------------
 * Computes the measurement operator unitary for a given basis Z, the qubit index to measure and the total number of qubits within the circuit.
 * 
 * a: basis in Z (0 or 1)
 * index: qubit index to measure
 * nb_qubits: total number of qubits in the circuit
 * 
 *  returns: measurement operator unitary
 */
struct Matrix *get_measurement_unitary(int a, size_t index, size_t nb_qubits)
{
    struct Matrix *observer = NULL;
    if (a == 0)
    {
        struct Matrix *ket_0 = init_matrix(1, 2);
        ket_0 = matrix_set_complex(ket_0, 1, 0, 0, 0);
        struct Matrix *ket_0_T = matrix_transpose(ket_0);
        observer = matrix_mul(ket_0, ket_0_T);
        free_matrix(ket_0);
        free_matrix(ket_0_T);
    }
    else
    {
        struct Matrix *ket_1 = init_matrix(1, 2);
        ket_1 = matrix_set_complex(ket_1, 1, 0, 0, 1);
        struct Matrix *ket_1_T = matrix_transpose(ket_1);
        observer = matrix_mul(ket_1, ket_1_T);
        free_matrix(ket_1);
        free_matrix(ket_1_T);
    }
    
    
    struct Matrix *id = identity(2);
    struct Matrix *measurement_unitary = identity(1);
    struct Matrix *old = measurement_unitary;
    for (size_t i = 0; i < nb_qubits; ++i)
    {
        if (i == index)
        {
            measurement_unitary = matrix_kron(old, observer);
        }
        else
        {
            measurement_unitary = matrix_kron(old, id);
        }
        free_matrix(old);
        old = measurement_unitary;
    }
    free_matrix(id);
    free_matrix(observer);
    return measurement_unitary;
}

/*
 * Function:  measure_all
 * --------------------
 * Measure every qubits in the Z basis. Return the measurement results as a List of Results.
 * 
 * state: the final state to be measured
 * nb_qubits: number of qubits
 * shots: trials number of the circuit running
 * 
 *  returns: List of Results
 */
struct List *measure_all(struct Matrix *state, size_t nb_qubits, size_t shots)
{
    srand(time(NULL));
    struct List *results = init_list(sizeof(struct Result*));
    struct Matrix *measurement_unit = NULL;
    
    for (size_t i = 0; i < nb_qubits; ++i)
    {
        print_matrix(state);
        printf("\n");
        measurement_unit = get_measurement_unitary(0, i, nb_qubits);

        // TODO: implement scalar product between matrices.
        struct Matrix *measurement_unit_transconj = matrix_transconj(measurement_unit);
        struct Matrix *state_bra = matrix_transconj(state);
        struct Matrix *left = matrix_mul(state_bra, measurement_unit_transconj);
        free_matrix(measurement_unit_transconj);
        free_matrix(state_bra);
        struct Matrix *right = matrix_mul(measurement_unit, state);
        struct Matrix *proba_mat = matrix_mul(right, left);
        free_matrix(left);
        free_matrix(right);
        struct Complex *p0 = proba_mat->matrix[0][0];
        // TODO: implement scalar product between matrices.

        const double proba_zero = *p0->a;
        const double proba_one = 1 - *p0->a;
        free_matrix(proba_mat);
        struct Result *res_i = NULL;
        if (shots > 0)
        {
            res_i = init_result(proba_zero, proba_one, i, bernoulli(proba_one));
            if (res_i->bit == 1)
            {
                free_matrix(measurement_unit);
                measurement_unit = get_measurement_unitary(1, i, nb_qubits);
            }
        }
        else
        {
            res_i = init_result(proba_zero, proba_one, i, -1);
        }
        struct Matrix *new_state = matrix_mul(measurement_unit, state);
        free_matrix(state);
        
        if (proba_zero != 0)
        {
            struct Complex *complex_scal = init_complex(1 / sqrt(proba_zero), 0);
            new_state = matrix_scal_mul(complex_scal, new_state);
            free_complex(complex_scal);
        }
        state = new_state;
        free_matrix(measurement_unit);
        results = list_append(results, res_i);
    }
    free_matrix(state);
    return results;
}

struct List *run_emulation(struct Circuit *circuit, size_t shots)
{
    // Get the whole system unitary
    struct Matrix *system_unitary = get_system_unitary(circuit);
    struct Matrix *state_transpose = matrix_transpose(circuit->vector_state);

    // Get the final vector state
    struct Matrix *state = matrix_mul(system_unitary, circuit->vector_state);

    free_matrix(state_transpose);
    free_matrix(system_unitary);    
    
    // Measure all qubits
    struct List *results = measure_all(state, circuit->nb_qubits, shots);

    return results;
}
