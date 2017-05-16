
#include <stdio.h>
#include "life.h"
#include "draw.h"

/**
 */
void draw (grid_t *g)
{
  uint32_t x, y, d;
  d = g->d;
  for (x = 0; x < d; x++) {
    for (y = 0; y < d; y++) {
      printf("%c", g->m[x + d * y] ? 'x' : '_');
    }
    switch (x)
    {
      case 0: printf(" i: %u", g->i); break;
      case 1: printf(" t: %u", g->e.t); break;
      case 2: printf(" b: %u", g->e.b); break;
      case 3: printf(" l: %u", g->e.l); break;
      case 4: printf(" r: %u", g->e.r); break;
    }
    printf("\n");
  }
}
