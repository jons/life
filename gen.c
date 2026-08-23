/**
 * self-programming game of life
 * 
 * random grid generator
 *
 * usage: gen N P
 *   N  grid dimension, an integer greater than zero
 *   P  probability, a decimal 0 <= P <= 1 that any given cell is '1'
 *
 * writes an N by N grid of '1' and '0' characters to standard output,
 * one row per line, N characters per line. the default value of a cell
 * is '0'.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * generate and print an N by N grid to stdout
 * each cell is '1' with probability P and '0' otherwise
 */
int gen (int n, double p)
{
    int x, y;

    for (y = 0; y < n; y++)
    {
        for (x = 0; x < n; x++)
        {
            /* rand() / (RAND_MAX + 1.0) is uniform over [0, 1) */
            double r = (double)rand() / ((double)RAND_MAX + 1.0);
            putchar(r < p ? '1' : '0');
        }
        putchar('\n');
    }

    return 0;
}

/**
 */
int main (int argc, char **argv)
{
    char *end;
    long n;
    double p;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s N P\n", argv[0]);
        fprintf(stderr, "  N  grid dimension, an integer greater than zero\n");
        fprintf(stderr, "  P  probability, 0 <= P <= 1, that a cell is '1'\n");
        return 1;
    }

    n = strtol(argv[1], &end, 10);
    if (*end != '\0' || n <= 0)
    {
        fprintf(stderr, "%s: N must be an integer greater than zero\n", argv[0]);
        return 1;
    }

    p = strtod(argv[2], &end);
    if (*end != '\0' || p < 0.0 || p > 1.0)
    {
        fprintf(stderr, "%s: P must be a decimal between 0 and 1 inclusive\n", argv[0]);
        return 1;
    }

    srand((unsigned)time(NULL));

    return gen((int)n, p);
}
