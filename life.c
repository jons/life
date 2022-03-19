/**
 * self-programming game of life
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "life.h"
#include "grid.h"
#include "prog.h"
#include "compiler.h"
#include "draw.h"


/**
 */
int execute (prog_t *p, grid_t *g, uint8_t *v)
{
    p->c = 0;
    p->r = 0;

}

/**
 */
void life_step (grid_t *next, grid_t *current)
{
    uint8_t v;
    uint32_t x, y, d;
    int r;
    prog_t e;
    compiler_t c;

    assert(current->d == next->d);
    assert(current->d > 0);
    assert(current->m != NULL);
    assert(next->m != NULL);

    d = current->d;
    for (y = 0; y < d; y++)
    {
        for (x = 0; x < d; x++)
        {
            if (x == 1 && y == 2)// xxx
            {
                prog_init(&e);
                compiler_init(&c, &e);
                r = compiler_readinfo(&c, current, x, y);
                assert(r);// xxx
                list(&e);
                r = execute(&e, current, &v);
                prog_free(&e);
                assert(r);// xxx
                printf("result: %d\n", v);
                grid_write(next, x, y, v);
                break;// xxx
            }
        }
    }
}