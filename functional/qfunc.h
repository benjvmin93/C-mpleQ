#ifndef QFUNC_H
#define QFUNC_H

#include "../emulator/circuit.h"
#include "../emulator/gates.h"
#include "../emulator/emulator.h"

/*
 * Gate functions abstraction
 */
void rx(struct Circuit *circuit, double rotation, size_t target);
void ry(struct Circuit *circuit, double rotation, size_t target);
void rz(struct Circuit *circuit, double rotation, size_t target);
void h(struct Circuit *circuit, size_t target);
void x(struct Circuit *circuit, size_t target);
void y(struct Circuit *circuit, size_t target);
void z(struct Circuit *circuit, size_t target);
void cGate(struct Circuit *circuit, int gate, size_t control, size_t target);

#endif