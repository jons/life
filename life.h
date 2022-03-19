/**
 * self-programming game of life
 */
#ifndef LIFE_H
#define LIFE_H
#include <stdint.h>
#include "grid.h"
#include "prog.h"

typedef struct hist hist_t;

struct hist
{
  uint32_t dt;  // number of changes from previous grid
  uint32_t i;   // current iteration
};

/**
 * perform a single iteration of life on the current grid, writing
 * the new universe to the next grid
 * 
 * grids must be the same dimensions
 * grow each grid by one new ring
 * compile the programs for each cell of grid 'current'
 * run them and write the output into grid 'next'
 */
void life_step (grid_t *next, grid_t *current);


#endif