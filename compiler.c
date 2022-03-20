/**
 * self-programming game of life
 * 
 * compile a local grid region to a program
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "compiler.h"
#include "draw.h"

/**
 */
void compiler_init (compiler_t *c, prog_t *e)
{
    c->e = e;
    c->jr = 0;
}

/**
 */
int compiler_append (compiler_t *c, rule_t *r)
{
    int s;
    uint8_t range = 0;
    s = prog_append(c->e, r);
    if (s)
    {
        if (c->jr > 0)
            c->jr--;

        switch (r->ret.in)
        {
            case PI_RET:
                range = r->ret.op;
                break;
            case PI_CJMP:
                range = r->cjmp.pci;
                break;
        }
        // will this new jump range extend the current range?
        if (range > 0 && range > c->jr)
            c->jr = range;
    }
    return s;
}

/**
 * advance the read pointer around a square of dimension n
 * v is the current ring-read direction
 * the caller must have just read position <x,y>
 */
int compiler_readinfo_step (uint32_t n, uint32_t *v, uint32_t *x, uint32_t *y)
{
    assert(n > 1);
    switch (*v)
    {
        case 0: // right: 1, 0
            if (*x + 1 == n)
            {
                *v = 1;
                return compiler_readinfo_step(n, v, x, y);
            }
            (*x)++;
            break;

        case 1: // down: 0, 1
            if (*y + 1 == n)
            {
                *v = 2;
                return compiler_readinfo_step(n, v, x, y);
            }
            (*y)++;
            break;

        case 2: // left: -1, 0
            if (*x == 0)
            {
                *v = 3;
                return compiler_readinfo_step(n, v, x, y);
            }
            (*x)--;
            break;

        case 3: // up: 0, -1
            if (*y == 0)
            {
                *v = 0;
                return compiler_readinfo_step(n, v, x, y);
            }
            (*y)--;
            break;
    }
    return 1;
}


/**
 * interpret a region of grid G as a series of program instructions
 * 
 * feed instructions to a compiler that will append each to a program,
 * and indicate to this read loop once the program is haltable and no
 * additional instructions could be reached
 * 
 * read rings centered on real position <rx, ry>
 */
int compiler_readinfo (compiler_t *c, grid_t *g, uint32_t rx, uint32_t ry)
{
    uint32_t d, v, x, y;
    uint32_t i, j, b, r;
    uint8_t halt;
    rule_t rule;

    // count total number of bytes as we go
    i = 0;
    halt = 1;
    for (r = 0; halt; r++)
    {
        // dimension of current ring
        d = ((r + 1) << 1) + 1;
        // prepare ring-step state to move to the right
        v = 0;
        // place relative index at top dead center of current ring
        x = rx;
        y = ry - (r + 1);
        // read up to r bytes: each ring has one more byte than the last
        for (j = 0; j < (r + 1); j++)
        {
            i++;
            // move through all 8 bit positions in the current byte ring
            // read each cell into one bit of current rule
            rule.u8 = 0;
            for (b = 0; b < 8; b++)
            {
                if (grid_read(g, x, y))
                    rule.u8 |= (1 << (7 - b));
                compiler_readinfo_step(d, &v, &x, &y);
            }

            if (0 == compiler_append(c, &rule))
            {
                // malloc failed
                return 0;
            }

            // if the program has a jump range, we'll keep reading
            // otherwise, halt once we read RET (or JMP 0, same thing)
            if (c->jr == 0 && rule.ret.in == PI_RET && rule.ret.op == 0)
            {
                halt = 0;
                break;
            }
        }
    }
    return 1;
}