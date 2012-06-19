
#include <stdio.h>
#include "life.h"
#include "draw.h"

void draw(grid_t *g)
{
  uint32_t x, y, d;
  d = g->d;
  for (x = 0; x < d; x++) {
    for (y = 0; y < d; y++)
      printf("%hhu", g->m[x + d * y] ? 1 : 0);
    printf("\n");
  }
}
