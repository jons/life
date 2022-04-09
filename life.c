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
 * execute program P on grid G from position <x,y>
 * on success, sets the program return register to a value of 1 or 0, and returns a positive value.
 * on failure, program state is undefined, and returns zero.
 */
int life_execute_cell (prog_t *p, grid_t *g, uint32_t x, uint32_t y)
{
    uint8_t pc, pci;    // program counter and increment
    list_t *ip;         // instruction pointer
    int i, j;           // LOI offsets

    pc = 0;
    ip = p->head;
    p->c = 0; // neighbor-counter register
    p->r = 0; // return register

    while (ip != NULL)
    {
        pci = 1;

        switch(ip->r.ret.in)
        {
            case PI_RET:
                // if JMP, set pci to [op]
                // if RET, clear pci (same thing)
                pci = ip->r.ret.op;
                break;

            case PI_CJMP:
                // conditionally set pci
                if (p->c)
                    pci = ip->r.cjmp.pci;
                break;

            case PI_CMP:
                switch (ip->r.cmp.op)
                {
                    case CMP_OP_F:
                        p->r = 0;
                        break;
                    case CMP_OP_GT:
                        p->r = p->c > ip->r.cmp.k ? 1 : 0;
                        break;
                    case CMP_OP_LT:
                        p->r = p->c < ip->r.cmp.k ? 1 : 0;
                        break;
                    case CMP_OP_NEQ:
                        p->r = p->c != ip->r.cmp.k ? 1 : 0;
                        break;
                    case CMP_OP_EQ:
                        p->r = p->c == ip->r.cmp.k ? 1 : 0;
                        break;
                    case CMP_OP_GTE:
                        p->r = p->c >= ip->r.cmp.k ? 1 : 0;
                        break;
                    case CMP_OP_LTE:
                        p->r = p->c <= ip->r.cmp.k ? 1 : 0;
                        break;
                    case CMP_OP_T:
                        p->r = 1;
                        break;
                    default:
                        p->f = FA_CMP_OP;
                        return 0;
                }
                break;

            case PI_LOI:
                i = (int)x + prog_loi_offset(ip->r.loi.i);
                j = (int)y + prog_loi_offset(ip->r.loi.j);
                if (grid_read(g, i, j))
                    p->c = (p->c + 1) % 8;
                break;

            default:
                // todo: if we need a fault address, it's either [pc]
                // or [pc] times the word size, which is eight
                p->f = FA_PI;
                return 0;
        }

        // terminate when any instruction causes
        // the increment to be zero
        if (pci == 0)
            break;

        // advance instruction pointer ahead [pci] addresses
        while (ip != NULL && pci > 0)
        {
            pc++;
            pci--;
            ip = ip->next;
        }
    }
    return 1;
}

/**
 */
void life_execute (grid_t *next, grid_t *current)
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
            prog_init(&e);
            compiler_init(&c, &e);
            r = compiler_readinfo(&c, current, x, y);
            assert(r);// xxx
            list(&e);
            r = life_execute_cell(&e, current, x, y);
            v = e.r;
            prog_free(&e);
            assert(r);// xxx
            grid_write(next, x, y, v);
        }
    }
}