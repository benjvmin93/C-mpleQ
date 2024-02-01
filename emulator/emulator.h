#ifndef EMULATOR_H
#define EMULATOR_H

#include "../utils/matrix.h"
#include "circuit.h"

struct Result
{
    struct List *clbit_results;
    struct List *clbit_proba;
};

struct Result *init_result(struct Matrix *vector_state);
void free_result(struct Result *result);

struct Emulator
{
    struct Result *(*emulate_func)(struct Circuit *circuit, size_t shots);
};

struct Emulator *init_emulator(void);
void free_emulator(struct Emulator *emulator);
struct Result *run_emulation(struct Circuit *circuit, size_t shots);

#endif