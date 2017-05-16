/**
 * self-programming life
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "life.h"


/**
 * update extents of grid g when it has been marked live at <x, y>
 */
static void extents (grid_t *g, uint32_t x, uint32_t y)
{
  // initialize missing extents (default d) to write location
  // otherwise, compare
  if (g->d == g->e.t || x < g->e.t) // less than top
    g->e.t = x;

  if (g->d == g->e.b || x > g->e.b) // greater than bottom
    g->e.b = x;

  if (g->d == g->e.l || y < g->e.l) // y less than left
    g->e.l = y;

  if (g->d == g->e.r || y > g->e.r) // y greater than right
    g->e.r = y;
}


/**
 * write cell value v at <x,y> in grid g
 */
void wcell (grid_t *g, uint32_t x, uint32_t y, uint8_t v)
{
  uint8_t p;
  uint32_t d = g->d;
  if (x >= d || y >= d)
    return;
  g->m[x + d * y] = v;
  if (v)
    extents(g, x, y);
}


/**
 * read cell value from <x,y> of grid g
 */
uint8_t rcell (grid_t *g, uint32_t x, uint32_t y)
{
  uint32_t d = g->d;
  if (x >= d || y >= d)
    return 0;
  return g->m[x + d * y];
}


/**
 * read the instruction local to <x,y> in the grid g
 * computes the number of living neighbors, n, and the
 * rule bits, r
 */
void info (page_t *p, grid_t *g, uint32_t x, uint32_t y)
{
  ofst_t o[8] = {
    { .x = -1, .y = -1 },
    { .x =  0, .y = -1 },
    { .x =  1, .y = -1 },
    { .x =  1, .y =  0 },
    { .x =  1, .y =  1 },
    { .x =  0, .y =  1 },
    { .x = -1, .y =  1 },
    { .x = -1, .y =  0 },
  };
  uint8_t c, i, n = 0, r = 0;

  // 0 1 2
  // 7   3
  // 6 5 4

  memset(p, 0, sizeof(page_t));
  for (i = 0; i < 8; i++) {
    c = rcell(g, x + o[i].x, y + o[i].y);
    n += c;
    r = (r << 1) | c;
  }
  p->n = n;
  p->r.u8 = r;
}


/**
 *  compute the next grid cell from the current grid cell for location <x,y>
 */
void next_step (grid_t *n, grid_t *g, uint32_t x, uint32_t y)
{
  page_t p;
  info(&p, g, x, y);
  switch (p.r.bm.op)
  {
    case OP_GT:  if (p.r.bm.x > p.n)  { wcell(n, x, y, p.r.bm.f); return; } break;
    case OP_GTE: if (p.r.bm.x >= p.n) { wcell(n, x, y, p.r.bm.f); return; } break;
    case OP_LT:  if (p.r.bm.x < p.n)  { wcell(n, x, y, p.r.bm.f); return; } break;
    case OP_LTE: if (p.r.bm.x <= p.n) { wcell(n, x, y, p.r.bm.f); return; } break;
  }
  wcell(n, x, y, rcell(g, x, y));
}


/**
 * compute the next grid, n, from the current grid, g
 */
void next_grid (grid_t *n, grid_t *g)
{
  uint32_t x, y, d;
  d = g->d;
  n->i = g->i + 1;
  n->e.t = d;
  n->e.b = d;
  n->e.l = d;
  n->e.r = d;
  for (x = 0; x < d; x++)
    for (y = 0; y < d; y++)
      next_step(n, g, x, y);
}


/**
 * initialize a grid of dimension d
 */
int init_grid (grid_t *g, uint32_t d)
{
  uint32_t x, y;
  g->i = 0;
  g->d = d;
  g->e.t = d;
  g->e.b = d;
  g->e.l = d;
  g->e.r = d;
  g->m = malloc(d * d * sizeof(uint8_t));
  if (NULL == g->m)
    return 0;
  memset(g->m, 0, d * d * sizeof(uint8_t));
  return 1;
}


/**
 */
void free_grid (grid_t *g)
{
  free(g->m);
}
