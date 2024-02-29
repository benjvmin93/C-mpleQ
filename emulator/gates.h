#ifndef GATES_H
#define GATES_H

#include "../utils/matrix.h"
#include "../utils/list.h"

enum Gates
{
  RX = 0,
  RY,
  RZ,
  H,
  X,
  Y,
  Z,
  P,
  M0
};

struct Gate
{
    enum Gates id;
    struct Matrix *m_gate;
    struct List *controls;
    size_t target;
    char *name;
};

struct Gate *init_gate(enum Gates gate_id, double rotation, struct List *controls, size_t targets);
void free_gate(void *gate);
struct Matrix *get_control_U(enum Gates gate_id, double rotation, struct List *controls, size_t target);
struct Matrix *get_Rx(double rotation);
struct Matrix *get_Ry(double rotation);
struct Matrix *get_Rz(double rotation);
struct Matrix *get_gate_matrix(int gate_id, double rotation);
char *get_gate_name(int gate_id);
struct Matrix *build_unitary_gate(struct Gate *gate, size_t n_qubits);
struct Matrix *get_H(void);
struct Matrix *get_X(void);
struct Matrix *get_Y(void);
struct Matrix *get_Z(void);
struct Matrix *get_M_0(void);

#endif