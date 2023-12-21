#ifndef GATES_H
#define GATES_H

#include "../utils/matrix.h"
#include "../utils/list.h"

enum Gates
{
  RX = 0,
  RY,
  RZ,
  P,
  M
};

struct Gate
{
    struct Matrix *m_gate;
    struct List *controls;
    struct List *targets;
};

struct Gate *init_gate(enum Gates gate_id, double rotation, struct List *controls, struct List *targets);
void free_gate(struct Gate *gate);

struct Matrix *get_Rx(double rotation);
struct Matrix *get_Ry(double rotation);
struct Matrix *get_Rz(double rotation);
struct Matrix *get_gate_matrix(int gate_id, double rotation);
struct Matrix *build_unitary_gate(struct Gate *gate, size_t n_qubits);
struct Matrix *get_H(void);
struct Matrix *get_X(void);

#endif