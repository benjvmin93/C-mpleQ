#include "qfunc.h"
#include "../utils/list.h"
#include "../utils/alloc.h"

struct List *get_size_list(size_t data)
{
    size_t *d = xmalloc(sizeof(size_t));
    *d = data;
    struct List *L = init_list(sizeof(size_t));
    L = list_append(L, d);
    return L;
}

void rx(struct Circuit *circuit, double rotation, size_t target)
{
    struct List *T = get_size_list(target);
    circuit = circuit_add_gate(circuit, RX, rotation, NULL, T);
}

void ry(struct Circuit *circuit, double rotation, size_t target)
{
    struct List *T = get_size_list(target);
    circuit = circuit_add_gate(circuit, RY, rotation, NULL, T);
}

void rz(struct Circuit *circuit, double rotation, size_t target)
{
    struct List *T = get_size_list(target);
    circuit = circuit_add_gate(circuit, RZ, rotation, NULL, T);
}

void h(struct Circuit *circuit, size_t target)
{
    struct List *T = get_size_list(target);
    circuit = circuit_add_gate(circuit, H, 0, NULL, T);
}

void x(struct Circuit *circuit, size_t target)
{
    struct List *T = get_size_list(target);
    circuit = circuit_add_gate(circuit, X, 0, NULL, T);
}

void y(struct Circuit *circuit, size_t target)
{
    struct List *T = get_size_list(target);
    circuit = circuit_add_gate(circuit, Y, 0, NULL, T);
}

void z(struct Circuit *circuit, size_t target)
{
    struct List *T = get_size_list(target);
    circuit = circuit_add_gate(circuit, Z, 0, NULL, T);
}

void cGate(struct Circuit *circuit, int gate, size_t control, size_t target)
{
    struct List *T = get_size_list(target);
    struct List *C = get_size_list(control);
    circuit = circuit_add_gate(circuit, gate, 0, C, T);
}