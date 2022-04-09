/**
 * self-programming game of life
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "life.h"
#include "draw.h"

#define SZ (11)
#define PSZ (30)

/**
 * the values of the first PSZ primes less than or equal to SZ*SZ (121)
 */
int p[PSZ] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113 };


/**
 */
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


/**
 */
int main (int argc, char **argv)
{
    int x, y, i = 0, k;
    grid_t a, b, *v, *w;

    grid_init(&a, SZ);
    grid_init(&b, SZ);

    // LIVE! LIVE, DAMN YOU
    for (x = 0; x < SZ; x++)
    {
        for (y = 0; y < SZ; y++)
        {
            /*k = x * SZ + y;
            if (p[i] == k)
            {
                grid_write(&a, x, y, 1);
                i++;
                if (i == PSZ) i = 0;
            }*/

            if (x == 3 && y == 3)
            {
                grid_write(&a, x, y, 1);
            }
        }
    }

    v = &a;
    w = &b;
    x = 0;

    for (;;) {
        draw(v);
        //
        if (wait()) break;
        //
        life_execute(w, v);

        // flip-flop next/current grid on each pass
        if (x) { v = &a; w = &b; x = 0; }
        else   { v = &b; w = &a; x = 1; }
    }

    grid_free(&a);
    grid_free(&b);
    return 0;
}