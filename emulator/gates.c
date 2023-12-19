#include <math.h>

#include "gates.h"
#include "../utils/maths.h"
#include "../utils/alloc.h"

struct Gate *init_gate(enum Gates gate_id, double rotation, struct List *controls, struct List *targets)
{
    struct Gate *gate = xmalloc(sizeof(struct Gate));
    gate->m_gate = get_gate_matrix(gate_id, rotation);

    gate->controls = controls;
    gate->targets = targets;
    return gate;
}

void free_gate(struct Gate *gate)
{
    free_matrix(gate->m_gate);
    list_free(gate->targets, free);
    list_free(gate->controls, free);
    free(gate);
}

struct Matrix *get_Rx(double rotation)
{
    struct Matrix *Rx = init_matrix(2, 2);
    Rx = matrix_set_complex(Rx, mcos(rotation / 2), 0, 0, 0);
    Rx = matrix_set_complex(Rx, 0, -msin(rotation / 2), 0, 1);
    Rx = matrix_set_complex(Rx, 0, -msin(rotation / 2), 1, 0);
    Rx = matrix_set_complex(Rx, mcos(rotation / 2), 0, 1, 1);
    return Rx;
}
struct Matrix *get_Ry(double rotation)
{
    struct Matrix *Ry = init_matrix(2, 2);
    Ry = matrix_set_complex(Ry, mcos(rotation / 2), 0, 0, 0);
    Ry = matrix_set_complex(Ry, msin(rotation / 2), 0, 0, 1);
    Ry = matrix_set_complex(Ry, -msin(rotation / 2), 0, 1, 0);
    Ry = matrix_set_complex(Ry, mcos(rotation / 2), 0, 1, 1);
    return Ry;
}
struct Matrix *get_Rz(double rotation)
{
    struct Matrix *Rz = init_matrix(2, 2);
    Rz = matrix_set_complex(Rz, mcos(rotation / 2), -msin(rotation / 2), 0, 0);
    Rz = matrix_set_complex(Rz, mcos(rotation / 2), msin(rotation / 2), 1, 1);
    return Rz;
}

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
    default:
        break;
    }
    return m_gate;
}

struct Matrix *build_unitary_gate(struct Gate *gate, size_t n_qubits)
{
    // TODO : controls
    struct Matrix *id = identity(2);
    struct Matrix *system_unitary = identity(pow(2, n_qubits));
    size_t n_target = list_length(gate->targets);
    for (size_t target_i = 0; target_i < n_target; ++target_i)
    {
        size_t target = *(size_t*)list_at(gate->targets, target_i)->data;
        struct Matrix *unitary = NULL;
        struct Matrix *old = init_matrix(1, 1);
        old = matrix_set_complex(old, 1, 0, 0, 0);

        for (size_t i = 0; i < n_qubits; ++i)
        {
            if (i < target)
            {
                unitary = matrix_kron(old, id);
            }
            else if (i == target)
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
        free_matrix(old);
        system_unitary = matrix_mul(unitary, system_unitary);
    }
    free_matrix(id);
    return system_unitary;
}

struct Matrix *get_H(void)
{
    struct Matrix *Rx = get_gate_matrix(RX, M_PI);
    struct Matrix *Ry = get_gate_matrix(RY, M_PI / 2);

    struct Matrix *H = matrix_mul(Ry, Rx);
    free_matrix(Rx);
    free_matrix(Ry);
    return H;
}

struct Matrix *get_X(void)
{
    struct Matrix *X = get_gate_matrix(RX, M_PI);
    return X;
}