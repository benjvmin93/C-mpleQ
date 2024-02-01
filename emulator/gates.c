#include <math.h>
#include <stdio.h>

#include "gates.h"
#include "../utils/maths.h"
#include "../utils/complex.h"
#include "../utils/alloc.h"

/*
 * Function: init_gate
 * ------------------
 * initialize a gate
 * 
 * gate_id: gate enumeration
 * rotation: rotation value in gradient (optional)
 * controls: list of control qubits (optional)
 * targets: list of target qubits
 * 
 * returns: the gate structure according to the given arguments
 */
struct Gate *init_gate(enum Gates gate_id, double rotation, struct List *controls, struct List *targets)
{
    struct Gate *gate = xmalloc(sizeof(struct Gate));
    gate->m_gate = get_gate_matrix(gate_id, rotation);
    gate->id = gate_id;
    gate->controls = controls;
    gate->targets = targets;
    return gate;
}

void free_gate(void *gate)
{
    struct Gate *g = gate;
    free_matrix(g->m_gate);
    list_free(g->targets, free);
    list_free(g->controls, free);
    free(g);
}

/*
* Function: get_Rx
* ----------------
* 
* rotation: rotation in gradient
*
* returns: the rotation operator around X axis according to the given rotation
*/
struct Matrix *get_Rx(double rotation)
{
    struct Matrix *Rx = init_matrix(2, 2);

    Rx = matrix_set_complex(Rx, mcos(rotation / 2), 0, 0, 0);
    Rx = matrix_set_complex(Rx, 0, -msin(rotation / 2), 0, 1);
    Rx = matrix_set_complex(Rx, 0, -msin(rotation / 2), 1, 0);
    Rx = matrix_set_complex(Rx, mcos(rotation / 2), 0, 1, 1);
    return Rx;
}

/*
* Function: get_Ry
* ----------------
* 
* rotation: rotation in gradient
*
* returns: the rotation operator around Y axis according to the given rotation
*/
struct Matrix *get_Ry(double rotation)
{
    struct Matrix *Ry = init_matrix(2, 2);
    Ry = matrix_set_complex(Ry, mcos(rotation / 2), 0, 0, 0);
    Ry = matrix_set_complex(Ry, msin(rotation / 2), 0, 0, 1);
    Ry = matrix_set_complex(Ry, -msin(rotation / 2), 0, 1, 0);
    Ry = matrix_set_complex(Ry, mcos(rotation / 2), 0, 1, 1);
    return Ry;
}

/*
* Function: get_Rz
* ----------------
* 
* rotation: rotation in gradient
*
* returns: the rotation operator around Z axis according to the given rotation
*/
struct Matrix *get_Rz(double rotation)
{
    struct Matrix *Rz = init_matrix(2, 2);
    Rz = matrix_set_complex(Rz, mcos(rotation / 2), -msin(rotation / 2), 0, 0);
    Rz = matrix_set_complex(Rz, mcos(rotation / 2), msin(rotation / 2), 1, 1);
    return Rz;
}

/*
* Function: get_P
* ----------------
* 
* rotation: rotation in gradient
*
* returns: the phase operator according to the given rotation.
*/
struct Matrix *get_P(double rotation)
{
    struct Matrix *P = init_matrix(2, 2);
    P = matrix_set_complex(P, 1, 0, 0, 0);
    P = matrix_set_complex(P, mcos(rotation), msin(rotation), 1, 1);
    return P;
}

/*
* Function: get_M_0
* ----------------
*
* returns: the measurement operator M0 according to the computational basis {0, 1}
*/
struct Matrix *get_M_0(void)
{
    struct Matrix *ket_0 = init_matrix(1, 2);
    ket_0 = matrix_set_complex(ket_0, 1, 0, 0, 1);
    struct Matrix *bra_0 = matrix_transpose(ket_0);
    struct Matrix *M0 = matrix_kron(ket_0, bra_0);

    free_matrix(ket_0);
    free_matrix(bra_0);
    return M0;
}

/*
* Function: get_gate_matrix
* ----------------
* 
* gate_id: int id corresponding to the gate enumeration
* rotation: rotation in gradient
*
* returns: the matrix corresponding to the id according to the given rotation
*/
struct Matrix *get_gate_matrix(int gate_id, double rotation)
{
    struct Matrix *m_gate = NULL;
    switch (gate_id)
    {
    case RX:
        m_gate = get_Rx(rotation);
        break;
    case RY:
        m_gate = get_Ry(rotation);
        break;
    case RZ:
        m_gate = get_Rz(rotation);
        break;
    case P:
        m_gate = get_P(rotation);
        break;
    case M0:
        m_gate = get_M_0();
        break;
    case H:
        m_gate = get_H();
        break;
    case X:
        m_gate = get_X();
        break;
    case Y:
        m_gate = get_Y();
        break;
    case Z:
        m_gate = get_Z();
        break;
    default:
        break;
    }
    return m_gate;
}

/*
* Function: build_unitary_gate
* ----------------
* 
* gate: gate structure to turn into unitary gate
* n_qubits: number of qubits within the circuit
*
* returns: the unitary gate corresponding to the given gate within the circuit using the given number of qubits
*/
struct Matrix *build_unitary_gate(struct Gate *gate, size_t n_qubits)
{
    // TODO : controls
    struct Matrix *id = identity(2); // Initialize identity
    struct Matrix *gate_unitary = identity(pow(2, n_qubits)); // Initialize the gate unitary matrix
    size_t n_target = list_length(gate->targets);
    for (size_t target_i = 0; target_i < n_target; ++target_i) // Iterate through each target qubit
    {   
        struct List *target_list = list_at(gate->targets, target_i);
        size_t *target = (size_t*)target_list->data; // Get target qubit index
        struct Matrix *unitary = init_matrix(1, 1); // Initialize unitary gate
        unitary = matrix_set_complex(unitary, 1, 0, 0, 0);
        struct Matrix *old = unitary;
        for (size_t i = 0; i < n_qubits; ++i) // Iterate through each qubit to build unitary matrix
        {
            if (i < *target)
            {
                unitary = matrix_kron(old, id);
            }
            else if (i == *target)
            {
                unitary = matrix_kron(old, gate->m_gate);
            }
            else
            {
                unitary = matrix_kron(id, old);
            }
            free_matrix(old);
            old = unitary;
        }
        struct Matrix *old_gate_unitary = gate_unitary;
        gate_unitary = matrix_mul(old, old_gate_unitary);
        free_matrix(old);
        free_matrix(old_gate_unitary);
        old_gate_unitary = gate_unitary;
    }
    free_matrix(id);
    return gate_unitary;
}

/*
* Function: get_H
* ----------------
* Computes H as H = X.Ry(PI/2)
*
* returns: the Hadamard gate.
*/
struct Matrix *get_H(void)
{
    struct Matrix *X = get_X();
    struct Matrix *Ry = get_Ry(M_PI / 2);
    struct Matrix *H = matrix_mul(X, Ry);
    free_matrix(X);
    free_matrix(Ry);
    return H;
}

/*
* Function: get_X
* ----------------
* computes X as X = Ry(PI).P
*
* returns: the X gate.
*/
struct Matrix *get_X(void)
{
    struct Matrix *P = get_P(M_PI);
    struct Matrix *Ry = get_Ry(M_PI);
    struct Matrix *X = matrix_mul(Ry, P);
    free_matrix(P);
    free_matrix(Ry);

    return X;
}

/*
* Function: get_Z
* ----------------
* computes Z as Z = Rz(PI)
*
* returns: the Z gate.
*/
struct Matrix *get_Z(void)
{
    struct Matrix *Z = get_P(M_PI);
    return Z;
}

/*
* Function: get_Y
* ----------------
* computes Y as Y = Z.X
*
* returns: the Y gate, up to a global phase.
*/
struct Matrix *get_Y(void)
{
    struct Matrix *X = get_X();
    struct Matrix *Z = get_Z();
    struct Matrix *Y = matrix_mul(Z, X);

    free_matrix(X);
    free_matrix(Z);
    return Y;
}