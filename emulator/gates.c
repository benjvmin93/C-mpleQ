#include <math.h>

#include "gates.h"
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
    Rx = matrix_set_complex(Rx, cos(rotation / 2), 0, 0, 0);
    Rx = matrix_set_complex(Rx, 0, -sin(rotation / 2), 0, 1);
    Rx = matrix_set_complex(Rx, 0, -sin(rotation / 2), 1, 0);
    Rx = matrix_set_complex(Rx, cos(rotation / 2), 0, 1, 1);
    return Rx;
}
struct Matrix *get_Ry(double rotation)
{
    struct Matrix *Ry = init_matrix(2, 2);
    Ry = matrix_set_complex(Ry, cos(rotation / 2), 0, 0, 0);
    Ry = matrix_set_complex(Ry, sin(rotation / 2), 0, 0, 1);
    Ry = matrix_set_complex(Ry, -sin(rotation / 2), 0, 1, 0);
    Ry = matrix_set_complex(Ry, cos(rotation / 2), 0, 1, 1);
    return Ry;
}
struct Matrix *get_Rz(double rotation)
{
    struct Matrix *Rz = init_matrix(2, 2);
    Rz = matrix_set_complex(Rz, cos(rotation / 2), -sin(rotation / 2), 0, 0);
    Rz = matrix_set_complex(Rz, cos(rotation / 2), sin(rotation / 2), 1, 1);
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

/*
struct Matrix *build_unitary_gate(struct Gate *gate)
{
    return NULL;
}
*/
