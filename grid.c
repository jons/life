/**
 * self-programming game of life
 * 
 * the grid on which splife is played
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "grid.h"


/**
 * initialize grid G with dimension D
 * 
 * D must be a positive, odd number
 * grids are always square
 */
int grid_init (grid_t *g, uint32_t d)
{
    if (d <= 0)
        return 0;

    g->d = d;
    g->m = malloc(d * d * sizeof(uint8_t));
    if (NULL == g->m)
        return 0;
    memset(g->m, 0, d * d * sizeof(uint8_t));
    return 1;
}


/**
 * release memory allocated by grid_init. idempotent.
 */
void grid_free (grid_t *g)
{
    if (g->m != NULL)
        free(g->m);
    g->m = NULL;
    g->d = 0;
}


/**
 * add rows and columns to the outside edges of grid G
 * 
 * s must be a positive even number, to evenly distribute rows/columns
 * frees the old grid and allocates a new one
 * maintains the grid data in the center of the new grid
 */
int grid_extend (grid_t *g, uint8_t s)
{
    uint32_t d;
    uint8_t *m;
    uint8_t x, y;

    if (s <= 0 || s & 1)
        return 0;
    d = g->d + s;
    // uint32_t overflow check
    if (d <= g->d)
        return 0;
    m = malloc(d * d * sizeof(uint8_t));
    if (NULL == m)
        return 0;
    memset(m, 0, d * d * sizeof(uint8_t));
    // translate g->m into center of m
    for (y = 0; y < g->d; y++)
        for (x = 0; x < g->d; x++)
            m[(x + s) + (d * (y + s))] = g->m[x + (g->d * y)];
    free(g->m);
    g->m = m;
    g->d = d;
    return 1;
}


/**
 * write a cell state value at <x,y> in grid G
 * 
 * value written to cell is 1 (live) when v is 'truthy', which is all nonzero numbers
 * otherwise zero (dead) is written
 */
int grid_write (grid_t *g, uint32_t x, uint32_t y, uint8_t v)
{
    uint32_t d = g->d;
    if (x >= d || y >= d)
        return 0;
    g->m[x + d * y] = v ? 1 : 0;
    return 1;
}


/**
 * read cell state value at <x,y> in grid G
 * 
 * positions outside the grid's bounds are always zero (dead)
 * this is not success/failure code
 */
uint8_t grid_read (grid_t *g, uint32_t x, uint32_t y)
{
    uint32_t d = g->d;
    if (x >= d || y >= d)
        return 0;
    return g->m[x + d * y];
}