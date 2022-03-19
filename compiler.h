/**
 * self-programming game of life
 * 
 * compile a local grid region to a program
 */
#ifndef COMPILER_H
#define COMPILER_H
#include <stdint.h>
#include "grid.h"
#include "prog.h"

typedef struct compiler compiler_t;

/**
 */
void compiler_init (compiler_t *c, prog_t *e);

/**
 * add rule r to the program currently being "compiled" using state c.
 * 
 * when a jump instruction is appended to a program, the compiler updates how
 * much further it needs to read from the grid. normally it will terminate on
 * the first return instruction it finds, but the "jump range" will force it to
 * keep reading. this jump range is part of the compiler state. it's also the
 * only part of the compiler state.
 */
int compiler_append (compiler_t *c, rule_t *r);

/**
 * step forward by one cell along a spiral path in the grid
 * 
 * the path is centered on the cell whose program is being read
 * 
 * this function only calculates the next location in a grid of dimension N
 * v is the current state of the read loop
 * <x,y> is the previously read position
 */
int compiler_readinfo_step (uint32_t n, uint32_t *v, uint32_t *x, uint32_t *y);

/**
 * interpret grid G as an executable program
 * the program is compiled with compiler context C
 * 
 * return zero on failure
 */
int compiler_readinfo (compiler_t *c, grid_t *g, uint32_t rx, uint32_t ry);

#endif