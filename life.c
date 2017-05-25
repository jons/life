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
 * count neighbors of <x, y> in grid g
 */
uint8_t scan (grid_t *g, uint32_t x, uint32_t y)
{
  // 0 1 2
  // 7   3
  // 6 5 4
  static const ofst_t o[8] = {
    { .x = -1, .y = -1 },
    { .x =  0, .y = -1 },
    { .x =  1, .y = -1 },
    { .x =  1, .y =  0 },
    { .x =  1, .y =  1 },
    { .x =  0, .y =  1 },
    { .x = -1, .y =  1 },
    { .x = -1, .y =  0 },
  };
  uint8_t i, n = 0;
  for (i = 0; i < 8; i++) {
    n += rcell(g, x + o[i].x, y + o[i].y);
  }
  return n;
}


/**
 * interpret the current grid as a program
 */
void read_info (prog_t *e, grid_t *g)
{
  uint32_t x, y;
  uint8_t c, r = 0, i = 0;
  list_t *s;

  for (x = g->e.t; x <= g->e.b; x++) // top to bottom
  {
    if (x == g->d) break;
    for (y = g->e.l; y <= g->e.r; y++) // left to right
    {
      if (y == g->d) break;
      // control-break on i:
      // buffer cells until an instruction can be read
      if (i == 8)
      {
        s = (list_t *)malloc(sizeof(list_t));
        if (NULL != s)
        {
          s->r.u8 = r;
          //printf(" [%p] f=%u op=%u x=%u\n", s, s->r.bm.f, s->r.bm.op, s->r.bm.x);
          if (e->tail)
          {
            e->tail->next = s;
            e->tail = s;
          }
          else
          {
            e->head = e->tail = s;
          }
          s->next = e->head; // must come after e->head change
        }
        r = 0;
        i = 0;
      }
      //
      c = rcell(g, x, y);
      //printf("%u", c);
      r = (r << 1) | c;
      i++;
    }
  }

  // pad out buffer with zeroes
  // ensures we always have at least one instruction,
  // even if the grid is empty or 2x2
  while (i++ < 8)
  {
    //printf("0");
    r <<= 1;
  }

  s = (list_t *)malloc(sizeof(list_t));
  if (NULL != s)
  {
    s->r.u8 = r;
    //printf(" [%p] f=%u op=%u x=%u\n", s, s->r.bm.f, s->r.bm.op, s->r.bm.x);
    if (e->tail)
    {
      e->tail->next = s;
      e->tail = s;
    }
    else
    {
      e->head = e->tail = s;
    }
    s->next = e->head;
  }
}


/**
 */
void dump_info (prog_t *e)
{
  list_t *s = e->head;
  while (e->head)
  {
    e->tail = e->head->next;
    free(e->head);
    if (e->tail == s) break;
    e->head = e->tail;
  }
  e->head = NULL;
  e->tail = NULL;
}


/**
 * compute the next grid cell from the current grid cell for location <x,y>
 * return 1 if the new grid cell changed state, 0 if copied
 */
int next_step (grid_t *n, grid_t *g, rule_t *r, uint32_t x, uint32_t y)
{
  uint8_t v = scan(g, x, y);
  uint8_t p = rcell(g, x, y);
  int d = 0;
  switch (r->bm.op)
  {
    case OP_GT:  d = r->bm.x >  v; break;
    case OP_GTE: d = r->bm.x >= v; break;
    case OP_LT:  d = r->bm.x <  v; break;
    case OP_LTE: d = r->bm.x <= v; break;
  }
  if (d)
  {
    wcell(n, x, y, r->bm.f);
    return p != r->bm.f;
  }
  wcell(n, x, y, p);
  return 0;
}


/**
 * compute the next grid, n, from the current grid, g
 */
void next_grid (grid_t *n, grid_t *g)
{
  uint32_t x, y, d;
  uint32_t dt = 0;
  prog_t exe = { .head = NULL, .tail = NULL };
  list_t *c;

  d = g->d;
  n->i = g->i + 1;
  n->e.t = d;
  n->e.b = d;
  n->e.l = d;
  n->e.r = d;

  read_info(&exe, g);
  c = exe.head;
  for (x = 0; x < d; x++)
    for (y = 0; y < d; y++)
    {
      dt += next_step(n, g, &(c->r), x, y);
      c = c->next;
    }
  n->dt = dt;
  dump_info(&exe);
}


/**
 * initialize a grid of dimension d
 */
int init_grid (grid_t *g, uint32_t d)
{
  uint32_t x, y;
  g->i = 0;
  g->d = d;
  g->dt = 0;
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
