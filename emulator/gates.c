#include <math.h>
#include <stdio.h>
#include <string.h>

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
struct Gate *init_gate(enum Gates gate_id, double rotation, struct List *controls, size_t target)
{
    struct Gate *gate = xmalloc(sizeof(struct Gate));
    gate->target = target;
    gate->name = get_gate_name(gate_id);
    gate->id = gate_id;
    gate->controls = controls;
    if (controls)
    {
        gate->m_gate = get_control_U(gate_id, rotation, controls, target);
    }
    else
    {
        gate->m_gate = get_gate_matrix(gate_id, rotation);
    }
    
    return gate;
}

void free_gate(void *gate)
{
    struct Gate *g = gate;
    free_matrix(g->m_gate);
    list_free(g->controls, free);
    free(g->name);
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

bool indices_equal_function(void *a, void *b)
{
    size_t *a1 = a;
    size_t *b1 = b;

    return *a1 == *b1;
}

struct Matrix *get_control_U(enum Gates gate_id, double rotation, struct List *controls, size_t target)
{
    // Prepare |0><0| and |1><1| projectors
    struct Matrix *ket_0 = init_matrix(1, 2);
    ket_0 = matrix_set_complex(ket_0, 1, 0, 0, 0);
    struct Matrix *bra_0 = matrix_transpose(ket_0);
    struct Matrix *ket_1 = init_matrix(1, 2);
    ket_1 = matrix_set_complex(ket_1, 1, 0, 0, 1);
    struct Matrix *bra_1 = matrix_transpose(ket_1);
    struct Matrix *proj_ket_0 = matrix_mul(ket_0, bra_0);
    struct Matrix *proj_ket_1 = matrix_mul(ket_1, bra_1);
    free_matrix(ket_0);
    free_matrix(ket_1);
    free_matrix(bra_0);
    free_matrix(bra_1);

    // Sort control indices in ascendant order
    controls = mergeSort(controls);


    struct Matrix *id = identity(2);
    size_t *min_ctrl = controls->data;
    size_t *max_ctrl = list_at(controls, list_length(controls) - 1)->data;
    size_t min = target < *min_ctrl ? target : *min_ctrl;
    size_t max = target > *max_ctrl ? target : *max_ctrl;

    // Tensor product for the case when control == |0>
    struct Matrix *control_proj_0 = identity(1);
    struct Matrix *old_ctrl_proj = control_proj_0;
    for (size_t i = min; i <= max; ++i)
    {
        if (list_find(controls, &i, indices_equal_function) == NULL)
        {
            control_proj_0 = matrix_kron(old_ctrl_proj, id);
        }
        else
        {
            control_proj_0 = matrix_kron(old_ctrl_proj, proj_ket_0);
            free_matrix(proj_ket_0);
        }
        free_matrix(old_ctrl_proj);
        old_ctrl_proj = control_proj_0;
    }

    // Tensor product for the case when control == |1>
    struct Matrix *control_proj_1 = identity(1);
    old_ctrl_proj = control_proj_1;
    for (size_t i = min; i <= max; ++i)
    {
        if (i == target)
        {
            struct Matrix *gate = get_gate_matrix(gate_id, rotation);
            control_proj_1 = matrix_kron(old_ctrl_proj, gate);
            free_matrix(gate);
        }
        else if (list_find(controls, &i, indices_equal_function) == NULL)
        {
            control_proj_1 = matrix_kron(old_ctrl_proj, id);
            
        }
        else
        {
            control_proj_1 = matrix_kron(old_ctrl_proj, proj_ket_1);
            free_matrix(proj_ket_1);
        }
        free_matrix(old_ctrl_proj);
        old_ctrl_proj = control_proj_1;
    }
    struct Matrix *control_gate = matrix_add(control_proj_0, control_proj_1);
    free_matrix(id);
    
    free_matrix(control_proj_0);
    free_matrix(control_proj_1);
    return control_gate;
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
* Function: get_gate_name
* ----------------
* 
* gate_id: int id corresponding to the gate enumeration
*
* returns: the string corresponding to the name of the gate according to its id 
*/
char *get_gate_name(int gate_id)
{
    char *gate_name = NULL;
    char *name = "";
    size_t size = 0;
    switch (gate_id)
    {
    case RX:
        size = 2;
        name = "RX";
        break;
    case RY:
        size = 2;
        name = "RX";
        break;
    case RZ:
        size = 2;
        name = "RX";
        break;
    case P:
        size = 1;
        name = "P";
        break;
    case H:
        size = 1;
        name = "H";
        break;
    case X:
        size = 1;
        name = "X";
        break;
    case  Y:
        size = 1;
        name = "Y";
        break;
    case Z:
        size = 1;
        name = "Z";
        break;
    default:
        break;
    }

    gate_name = xmalloc(sizeof(char) * (size + 1));
    gate_name = strcpy(gate_name, name);
    return gate_name;
}

bool size_equal_func(size_t *a, size_t *b)
{
    return *a == *b;
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
    struct Matrix *id = identity(2); // Initialize identity
    struct Matrix *gate_unitary = identity(pow(2, n_qubits)); // Initialize the gate unitary matrix
    size_t target = gate->target;
    struct Matrix *unitary = init_matrix(1, 1); // Initialize unitary gate
    unitary = matrix_set_complex(unitary, 1, 0, 0, 0);
    struct Matrix *old = unitary;
    size_t *min_ctrl_gate = NULL;
    size_t *max_ctrl_gate = NULL;
    size_t min = target;
    size_t max = target;

    if (gate->controls)
    {
        struct List *ordered_controls = mergeSort(gate->controls);
        min_ctrl_gate = list_at(ordered_controls, 0)->data;
        max_ctrl_gate = list_at(ordered_controls, list_length(ordered_controls) - 1)->data;
        min = gate->target < *min_ctrl_gate ? gate->target : *min_ctrl_gate;
        max = gate->target > *max_ctrl_gate ? gate->target : *max_ctrl_gate;
    }
    
    for (size_t i = 0; i < n_qubits; ++i) // Iterate through each qubit to build unitary matrix
    {
        if (i < target && i < min)
        {
            unitary = matrix_kron(id, old);
        }
        else if (i == min)
        {
            unitary = matrix_kron(old, gate->m_gate);
            i = max;
        }
        else
        {
            unitary = matrix_kron(old, id);
        }
        free_matrix(old);
        old = unitary;
    }
    struct Matrix *old_gate_unitary = gate_unitary;
    gate_unitary = matrix_mul(old, old_gate_unitary);
    free_matrix(old);
    free_matrix(old_gate_unitary);
    old_gate_unitary = gate_unitary;
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