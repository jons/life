/**
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "life.h"
#include "draw.h"

#define SZ (12)
#define PSZ (32)
// primes <= SZ*SZ
int p[PSZ] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131 };


//
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


//
int main (int argc, char **argv)
{
  int x, y, i = 0, k;
  grid_t a, b, *v, *w;

  init_grid(&a, SZ);
  init_grid(&b, SZ);

  // LIVE! LIVE, DAMN YOU
  for (x = 0; x < SZ; x++)
    for (y = 0; y < SZ; y++)
    {
      k = x * SZ + y;
      if (p[i] == k)
      {
        wcell(&a, x, y, 1);
        i++;
        if (i == PSZ) i = 0;
      }
    }

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
