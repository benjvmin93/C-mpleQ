#ifndef GATES_H
#define GATES_H

#include "../utils/matrix.h"

enum Gates
{
  H = 0,
  X,
  Y,
  Z  
};

struct Gate
{
    enum Gates gate_id;
    bool is_controlled;
    size_t *control;
    size_t *target;
};

struct Gate *init_gate(enum Gates gate_id, bool is_controlled, size_t *control, size_t *target);
void free_gate(struct Gate *gate);

struct Matrix *get_H(void);
struct Matrix *get_X(void);
struct Matrix *get_Y(void);
struct Matrix *get_Z(void);
struct Matrix *get_gate(enum Gates gate);

#endif