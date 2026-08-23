/**
 * self-programming game of life
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "life.h"
#include "draw.h"
#include "load.h"

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
 * length of the first row of the text file at PATH
 * the loader does not size the grid, so the caller reads N from here first
 * returns -1 on failure
 */
uint32_t first_row_length (const char *path)
{
    FILE *f;
    uint32_t n = 0;
    int c;

    f = fopen(path, "r");
    if (NULL == f)
        return -1;

    for (;;)
    {
        c = fgetc(f);
        if ('\n' == c || '\r' == c || EOF == c)
            break;
        n++;
    }
    fclose(f);
    return n;
}

/**
 */
int main (int argc, char **argv)
{
    uint32_t n;
    int i = 0;
    grid_t a, b, *v, *w;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s grid.txt\n", argv[0]);
        return 1;
    }

    // the caller knows N, the length of the first row, and initializes first
    n = first_row_length(argv[1]);
    if (n == 0)
    {
        fprintf(stderr, "splife: no grid in %s\n", argv[1]);
        return 1;
    }

    grid_init(&a, n);
    grid_init(&b, n);

    if (!load_grid(&a, argv[1]))
    {
        fprintf(stderr, "splife: could not load grid from %s\n", argv[1]);
        grid_free(&a);
        grid_free(&b);
        return 1;
    }

    v = &a;
    w = &b;
    for (;;) {
        if (life_is_dead(v))
        {
            printf("died on step %d\n", i);
            break;
        }
        // user i/o
        //draw(v);
        //printf("%d\n", i);
        //if (wait()) break;
        // w <- v
        life_execute(w, v);

        // flip-flop next/current grid on each pass
        if (i & 1) { v = &a; w = &b; }
        else       { v = &b; w = &a; }
        i++;
    }

    grid_free(&a);
    grid_free(&b);
    return 0;
}