#include <math.h>

#include "gates.h"
#include "../utils/alloc.h"

struct Gate *init_gate(enum Gates gate_id, struct List *controls, struct List *targets)
{
    struct Gate *gate = xmalloc(sizeof(struct Gate));
    gate->gate_id = gate_id;
    gate->targets = targets;
    gate->controls = controls;
    return gate;
}

void free_gate(struct Gate *gate)
{
    list_free(gate->targets);
    list_free(gate->controls);
    free(gate);
}

struct Matrix *get_H(void)
{
    struct Matrix *H = init_matrix(2, 2);
    double coef = 1 / sqrt(2);
    H = matrix_set_complex(H, coef, 0, 0, 0);
    H = matrix_set_complex(H, coef, 0, 0, 1);
    H = matrix_set_complex(H, coef, 0, 1, 0);
    H = matrix_set_complex(H, -coef, 0, 1, 1);
    return H;
}

struct Matrix *get_X(void)
{
    struct Matrix *X = init_matrix(2, 2);
    X = matrix_set_complex(X, 1, 0, 0, 1);
    X = matrix_set_complex(X, 1, 0, 1, 0);
    return X;
}
struct Matrix *get_Y(void)
{
    struct Matrix *Y = init_matrix(2, 2);
    Y = matrix_set_complex(Y, 0, -1, 0, 1);
    Y = matrix_set_complex(Y, 0, 1, 1, 0);
    return Y;
}
struct Matrix *get_Z(void)
{
    struct Matrix *Z = init_matrix(2, 2);
    Z = matrix_set_complex(Z, 1, 0, 0, 0);
    Z = matrix_set_complex(Z, -1, 0, 1, 1);
    return Z;
}
struct Matrix *get_gate(enum Gates gate)
{
    struct Matrix *m_gate = NULL;
    switch (gate)
    {
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
