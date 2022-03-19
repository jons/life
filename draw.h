/**
 * self-programming game of life
 * 
 * stuff that helps you see what's going on
 */
#ifndef DRAW_H
#define DRAW_H
#include "grid.h"
#include "prog.h"

/**
 * draw grid G as an NxN square
 * 
 * distinguish live cells from dead ones with 'x' and '_' respectively
 */
void draw (grid_t *g);

/**
 * print instruction
 */
void list_single (rule_t *r);

/**
 * print the listing of executable program E
 */
void list (prog_t *e);

#endif