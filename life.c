/**
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "life.h"

void wcell(grid_t *g, uint32_t x, uint32_t y, uint8_t v)
{
  uint32_t d = g->d;
  if (x >= d || y >= d)
    return;
  g->m[x + d * y] = v;
}


uint8_t rcell(grid_t *g, uint32_t x, uint32_t y)
{
  uint32_t d = g->d;
  if (x >= d || y >= d)
    return 0;
  return g->m[x + d * y];
}


void info(page_t *p, grid_t *g, uint32_t x, uint32_t y)
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


void next_step(grid_t *n, grid_t *g, uint32_t x, uint32_t y)
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


void next_grid(grid_t *n, grid_t *g)
{
  uint32_t x, y, d;
  d = g->d;
  for (x = 0; x < d; x++)
    for (y = 0; y < d; y++)
      next_step(n, g, x, y);
}
