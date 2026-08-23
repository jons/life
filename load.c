/**
 * self-programming game of life
 * 
 * read grids from text files
 */
#include <stdio.h>
#include <stdlib.h>
#include "load.h"

/**
 * write row R of length LEN into grid G at row Y
 *
 * every row must be the same length as the grid's dimension
 * blank lines are tolerated only once the grid is complete
 * returns 1 on success, 0 on failure
 */
static int load_row (grid_t *g, const uint8_t *r, size_t len, uint32_t y)
{
    uint32_t d = g->d;
    uint32_t x;

    if (0 == len)
        return (y == d);

    if ((uint32_t)len != d || y >= d)
        return 0;

    for (x = 0; x < d; x++)
    {
        if ('0' == r[x])
            grid_write(g, x, y, 0);
        else if ('1' == r[x])
            grid_write(g, x, y, 1);
        else
            return 0;
    }
    return 1;
}

/**
 * load an N by N grid of '0' and '1' characters from the text file at PATH
 * into grid G, which must already be initialized to dimension N (the length
 * of the first row of the file). load_grid neither allocates nor frees G;
 * the caller is responsible for both grid_init before and grid_free after.
 * on failure G is left in place, possibly partially written.
 * returns 1 on success, 0 on failure
 */
int load_grid (grid_t *g, const char *path)
{
    FILE *f;
    uint8_t *row = NULL;
    size_t rcap = 0, rlen = 0;
    uint32_t y = 0;
    int c;

    f = fopen(path, "r");
    if (NULL == f)
        return 0;

    for (;;)
    {
        c = fgetc(f);
        if (EOF == c)
        {
            if (rlen > 0)
            {
                if (!load_row(g, row, rlen, y))
                {
                    free(row);
                    fclose(f);
                    return 0;
                }
                y++;
            }
            break;
        }

        if ('\n' == c)
        {
            if (!load_row(g, row, rlen, y))
            {
                free(row);
                fclose(f);
                return 0;
            }
            if (rlen > 0)
                y++;
            rlen = 0;
            continue;
        }

        if ('\r' == c)
        {
            /* consume an optional LF that follows a CR */
            c = fgetc(f);
            if ('\n' != c && EOF != c)
                ungetc(c, f);
            if (!load_row(g, row, rlen, y))
            {
                free(row);
                fclose(f);
                return 0;
            }
            if (rlen > 0)
                y++;
            rlen = 0;
            continue;
        }

        /* ordinary character: append it to the current row */
        if (rlen == rcap)
        {
            size_t ncap = rcap ? 2 * rcap : 64;
            uint8_t *nrow;
            if (ncap < rcap)
                nrow = NULL; // capacity overflow
            else
                nrow = (uint8_t *)realloc(row, ncap);
            if (NULL == nrow)
            {
                free(row);
                fclose(f);
                return 0;
            }
            row = nrow;
            rcap = ncap;
        }
        row[rlen++] = (uint8_t)c;
    }

    free(row);
    fclose(f);

    if (y != g->d)
        return 0;
    return 1;
}
