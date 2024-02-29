#ifndef EMULATOR_H
#define EMULATOR_H

#include "../utils/matrix.h"
#include "circuit.h"

struct Result
{
    double p0;
    double p1;
    size_t measure_index;
    int bit;
};

struct Result *init_result(const double p0, const double p1, const size_t measure_index, const int bit);
void free_result(void *result);
void free_results(struct List *results);

struct Emulator
{
    struct List *(*emulate_func)(struct Circuit *circuit, size_t shots);
};

struct Emulator *init_emulator(void);
void free_emulator(struct Emulator *emulator);
struct List *run_emulation(struct Circuit *circuit, size_t shots);
void print_results(struct List *results);

#endif