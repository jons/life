/**
 * self-programming game of life
 * 
 * the grid on which splife is played
 */
#ifndef GRID_H
#define GRID_H
#include <stdint.h>

typedef struct grid grid_t;

struct grid
{
    uint32_t d;   // dimension of the grid
    uint8_t *m;   // grid memory
};

/**
 */
int grid_init (grid_t *g, uint32_t d);

/**
 */
void grid_free (grid_t *g);

/**
 */
int grid_extend (grid_t *g, uint8_t s);

/**
 * write value V to grid G at <x,y>
 */
int grid_write (grid_t *g, uint32_t x, uint32_t y, uint8_t v);

/**
 * read grid G at <x,y> and return value
 */
uint8_t grid_read (grid_t *g, uint32_t x, uint32_t y);

#endif