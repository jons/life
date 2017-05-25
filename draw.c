
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
      case 0: printf("  i: %u", g->i); break;
      case 1: printf(" dt: %u", g->dt); break;

      case 3: printf("  T: %u", g->e.t); break;
      case 4: printf("  B: %u", g->e.b); break;
      case 5: printf("  L: %u", g->e.l); break;
      case 6: printf("  R: %u", g->e.r); break;
    }
    printf("\n");
  }
}
