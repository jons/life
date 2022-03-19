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
    //printf("step[%u][%u]:", n, *v);
    switch (*v)
    {
        case 0: // right: 1, 0
            if (*x + 1 == n)
            {
                //printf("turn\n");
                *v = 1;
                return compiler_readinfo_step(n, v, x, y);
            }
            else
            {
                //printf("right");
                (*x)++;
            }
            break;

        case 1: // down: 0, 1
            if (*y + 1 == n)
            {
                //printf("turn\n");
                *v = 2;
                return compiler_readinfo_step(n, v, x, y);
            }
            else
            {
                //printf("down");
                (*y)++;
            }
            break;

        case 2: // left: -1, 0
            if (*x == 0)
            {
                //printf("turn\n");
                *v = 3;
                return compiler_readinfo_step(n, v, x, y);
            }
            else
            {
                //printf("left");
                (*x)--;
            }
            break;

        case 3: // up: 0, -1
            if (*y == 0)
            {
                //printf("turn\n");
                *v = 0;
                return compiler_readinfo_step(n, v, x, y);
            }
            else
            {
                //printf("up");
                (*y)--;
            }
            break;

        default:
            return 1;
    }
    //printf(":%u,%u\n", *x, *y);
    return 0;
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
    uint8_t halt, k;
    rule_t rule;

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
            //printf(">readinfo:byte:%u\n", i);
            // move through all 8 bit positions in the ring
            k = 0;
            for (b = 0; b < 8; b++)
            {
                k <<= 1;
                if (grid_read(g, x, y))
                    k = k | 1;

                // ring-step to next bit
                compiler_readinfo_step(d, &v, &x, &y);
            }

            // interpret and "compile" instruction, append to program
            rule.u8 = k;
            //list_single(&rule);
            //printf("\n");
            if (0 == compiler_append(c, &rule))
            {
                // malloc failed
                return 0;
            }

            // if the program has a jump range, we'll keep reading
            // otherwise, halt once we read RET (or JMP 0, same thing)
            //printf(">readinfo:jumprange:%u\n", c->jr);
            //printf(">readinfo:op:%u\n", rule.ret.op);
            if (c->jr == 0 && rule.ret.in == PI_RET && rule.ret.op == 0)
            {
                //printf("halting\n");
                halt = 0;
                break;
            }
            //printf("\n");
        }
    }
    return 1;
}