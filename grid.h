/**
 * self-programming game of life
 * 
 * the grid on which splife is played
 */
#ifndef GRID_H
#define GRID_H
#include <stdint.h>

typedef struct grid grid_t;
typedef struct ofst ofst_t;

struct grid
{
  uint32_t d;   // dimension of the grid
  uint8_t *m;   // grid memory
};

struct ofst
{
  int8_t i; // these values are 1, 0, or -1
  int8_t j; // they have been converted with prog_loi_offset
};


int grid_init (grid_t *g, uint32_t d);

void grid_free (grid_t *g);

int grid_extend (grid_t *g, uint8_t s);

int grid_write (grid_t *g, uint32_t x, uint32_t y, uint8_t v);

uint8_t grid_read (grid_t *g, uint32_t x, uint32_t y);

#endif