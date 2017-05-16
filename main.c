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

  init_grid(&a, SZ);
  init_grid(&b, SZ);

  // LIVE! LIVE, DAMN YOU
  wcell(&a, 0, 0, 1);
  wcell(&a, 0, 1, 1);
  wcell(&a, 1, 0, 1);
  wcell(&a, 2, 0, 1);
  wcell(&a, 4, 4, 1);
  wcell(&a, 5, 5, 1);
  wcell(&a, 5, 6, 1);
  wcell(&a, 6, 6, 1);
  wcell(&a, 8, 6, 1);
  wcell(&a, 6, 10, 1);
  wcell(&a, 7, 10, 1);


  // flip-flop next/current grid
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

  free_grid(&a);
  free_grid(&b);
  return 0;
}
