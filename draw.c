/**
 * self-programming game of life
 */
#include <stdio.h>
#include "draw.h"
#include "prog.h"

/**
 */
void draw (grid_t *g)
{
    uint32_t x, y, d;
    d = g->d;
    for (y = 0; y < d; y++)
    {
        for (x = 0; x < d; x++)
        {
            printf("%c", g->m[x + d * y] ? 'x' : '_');
        }
        printf("\n");
    }
}

/**
 */
void list_single (rule_t *r)
{
    int i, j;
    uint8_t ignk; // ignore k

    switch (r->ret.in)
    {
        case PI_RET:
            if (r->ret.op)
                printf("_JMP +%08x", r->ret.op);
            else
                printf("_RET");
            break;

        case PI_CJMP:
            printf("CJMP +%08x", r->cjmp.pci);
            break;

        case PI_CMP:
            ignk = 0;
            printf("CMP ");
            switch (r->cmp.op)
            {
                case CMP_OP_F:    // 000 false
                    printf("F");
                    ignk = 1;
                    break;
                case CMP_OP_GT:   // 001
                    printf("Rc  >");
                    break;
                case CMP_OP_LT:   // 010
                    printf("Rc  <");
                    break;
                case CMP_OP_NEQ:  // 011
                    printf("Rc <>");
                    break;
                case CMP_OP_EQ:   // 100
                    printf("Rc ==");
                    break;
                case CMP_OP_GTE:  // 101
                    printf("Rc >=");
                    break;
                case CMP_OP_LTE:  // 110
                    printf("Rc <=");
                    break;
                case CMP_OP_T:    // 111 true
                    printf("T");
                    ignk = 1;
                    break;
            }
            if (!ignk)
                printf(" +%02x", r->cmp.k);
            break;

        case PI_LOI:
            i = prog_loi_offset(r->loi.i);
            j = prog_loi_offset(r->loi.j);
            printf("_LOI Rc [%d][%d]", i, j);
            break;

        default:
            // shouldn't be possible unless code's broken
            printf("_ERR [%08x] [%c%c] %c %c %c %c %c %c",
                r->u8,
                r->u8 & 0x80 ? '1' : '0',
                r->u8 & 0x40 ? '1' : '0',
                // the rest of it
                r->u8 & 0x20 ? '1' : '0',
                r->u8 & 0x10 ? '1' : '0',
                r->u8 & 0x08 ? '1' : '0',
                r->u8 & 0x04 ? '1' : '0',
                r->u8 & 0x02 ? '1' : '0',
                r->u8 & 0x01 ? '1' : '0');
            break;
    }
}

/**
 */
void list (prog_t *e)
{
    uint32_t pc = 0;
    list_t *n = e->head;

    while (n)
    {
        printf("[%08x] ", pc);
        list_single(&(n->r));
        printf("\n");
        pc += 8;
        n = n->next;
    }
}