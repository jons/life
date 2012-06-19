/**
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "life.h"
#include "draw.h"

#define SZ (11)


int wait()
{
  for (;;)
  {
    switch (getchar())
    {
      case EOF:
        perror("getchar");
      case 'q':
        return 1;

      case '\r':
      case '\n':
        return 0;
    }
  }
  return -1;
}


int main (int argc, char **argv)
{
  int x;
  grid_t a, b, *v, *w;   

  memset(&a, 0, sizeof(grid_t));
  memset(&b, 0, sizeof(grid_t));
  a.d = b.d = SZ;
  a.m = malloc(SZ*SZ);
  b.m = malloc(SZ*SZ);
  memset(a.m, 0, (SZ*SZ));
  memset(b.m, 0, (SZ*SZ));

  // LIVE, DAMN YOU
  a.m[5 + 11 * 5] = 1;
  a.m[5 + 11 * 6] = 1;
  a.m[6 + 11 * 6] = 1;

  v = &a; w = &b; x = 0;
  for (;;) {
    draw(v);
    //
    if (wait()) break;
    //
    next_grid(w, v);
    //
    if (x) { v = &a; w = &b; x = 0; }
    else   { v = &b; w = &a; x = 1; }
  }

  free(a.m);
  free(b.m);
  return 0;
}
