/**
 * self-programming game of life
 * 
 * unit tests
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "grid.h"
#include "draw.h"
#include "prog.h"
#include "compiler.h"
#include "life.h"

/**
 * check expected equals actual: list_t*
 */
void chk_eq_list(const char *t, list_t* e, list_t* a)
{
    if (e == a)
        printf("PASS %s\n", t);
    else
    {
        printf("FAIL %s, ", t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
        printf("0x%016lx != 0x%016lx\n", e, a);
#pragma GCC diagnostic pop
    }
}

/**
 * check expected does-not-equal actual: list_t*
 */
void chk_ne_list(const char *t, list_t* e, list_t* a)
{
    if (e != a)
        printf("PASS %s\n", t);
    else
    {
        printf("FAIL %s, ", t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
        printf("0x%016lx != 0x%016lx\n", e, a);
#pragma GCC diagnostic pop
    }
}

/**
 * check expected does-not-equal actual: rule_t* address
 */
void chk_ne_rulep(const char *t, rule_t* e, rule_t* a)
{
    if (e != a)
        printf("PASS %s\n", t);
    else
    {
        printf("FAIL %s, ", t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
        printf("0x%016lx != 0x%016lx\n", e, a);
#pragma GCC diagnostic pop
    }
}

/**
 * check expected equals actual: int
 */
void chk_eq_i(const char *t, int e, int a)
{
    if (e == a)
        printf("PASS %s\n", t);
    else
        printf("FAIL %s, %d != %d\n", t, e, a);
}

/**
 * check expected equals actual: uint32_t
 */
void chk_eq_u32(const char *t, uint32_t e, uint32_t a)
{
    if (e == a)
        printf("PASS %s\n", t);
    else
        printf("FAIL %s, %u != %u\n", t, e, a);
}

/**
 * check expected equals actual: uint8_t
 */
void chk_eq_u8(const char *t, uint8_t e, uint8_t a)
{
    if (e == a)
        printf("PASS %s\n", t);
    else
        printf("FAIL %s, %d != %d\n", t, e, a);
}

/**
 * check expected equals actual: uint8_t*
 */
void chk_eq_u8p(const char *t, uint8_t* e, uint8_t* a)
{
    if (e == a)
        printf("PASS %s\n", t);
    else
    {
        printf("FAIL %s, ", t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
        printf("0x%016lx != 0x%016lx\n", e, a);
#pragma GCC diagnostic pop
    }
}

/**
 * check expected does-not-equal actual: uint8_t*
 */
void chk_ne_u8p(const char *t, uint8_t* e, uint8_t* a)
{
    if (e != a)
        printf("PASS %s\n", t);
    else
    {
        printf("FAIL %s, ", t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
        printf("0x%016lx != 0x%016lx\n", e, a);
#pragma GCC diagnostic pop
    }
}


/**
 * helper that inspects a program listing
 * 
 * retrieve length of listing; number of rules
 * retrieve address of tail
 */
int help_inspect_listing(prog_t *e, list_t **tail)
{
    list_t *p = e->head;
    int c = 0;
    *tail = NULL;
    while (p)
    {
        if (p->next == NULL) // find tail
            *tail = p;
        p = p->next;
        c++;
    }
    return c;
}


/**
 * unit test everything
 *
 * console output indicates tests PASS or FAIL
 * a failed test outputs expected and actual to demonstrate the failure condition
 * assertions fail only when tests cannot continue; these are not tests themselves
 */
int main (int argc, char **argv)
{
    int rv;
    uint8_t v;
    grid_t g;
    compiler_t c;
    prog_t e;
    rule_t r;
    list_t *tp; // tail pointer

    // TEST: GRID

    // grid cannot start dimensionless
    g.d = 12345;
    g.m = (uint8_t *)0xdeadbeef;
    rv = grid_init(&g, 0);
    chk_eq_i("grid_init: zero dimension", 0, rv);
    chk_eq_u32("grid_init: zero dimension: untouched value d", 12345, g.d);
    chk_eq_u8p("grid_init: zero dimension: untouched value m", (uint8_t *)0xdeadbeef, g.m);

    // init grid
    rv = grid_init(&g, 1);
    chk_eq_i("grid_init: positive dimension", 1, rv);
    chk_eq_u32("grid_init: touched value d", 1, g.d);
    chk_ne_u8p("grid_init: touched value m", NULL, g.m);

    // every position in grid should be dead
    v = grid_read(&g, 0, 0);
    chk_eq_u8("grid_read: initial value", 0, v);

    // should be able to write to valid grid position
    rv = grid_write(&g, 0, 0, 1);
    chk_eq_i("grid_write: can write in bounds", 1, rv);

    // cannot write outside bounds
    rv = grid_write(&g, 0, 1, 0);
    chk_eq_i("grid_write: out-of-bounds X (1,0)", 0, rv);
    rv = grid_write(&g, 1, 0, 0);
    chk_eq_i("grid_write: out-of-bounds Y (0,1)", 0, rv);
    rv = grid_write(&g, (uint32_t)-1, (uint32_t)-1, 0);
    chk_eq_i("grid_write: way out-of-bounds (-1,-1)", 0, rv);

    // last written position not lost
    v = grid_read(&g, 0, 0);
    chk_eq_u8("grid_read: read previous write", 1, v);

    // can free grid
    grid_free(&g);
    chk_eq_u32("grid_free: reset value d", 0, g.d);
    chk_eq_u8p("grid_free: reset value m", NULL, g.m);
    // can free grid again
    grid_free(&g);
    chk_eq_u32("grid_free: unchanged d", 0, g.d);
    chk_eq_u8p("grid_free: unchanged m", NULL, g.m);

    // TEST: PROGRAMS

    // initializer touches everything
    e.c = 5;
    e.r = 1;
    e.head = (list_t *)0xfeddad;
    e.tail = (list_t *)0xdeadbeef;
    prog_init(&e);
    chk_eq_u8("prog_init: zeroed result register", 0, e.r);
    chk_eq_u8("prog_init: zeroed counter register", 0, e.c);
    chk_eq_list("prog_init: empty listing", NULL, e.head);
    chk_eq_list("prog_init: empty listing", NULL, e.tail);

    // deallocation also touches everything
    // crash if we can't free an empty program
    e.c = 5;
    e.r = 1;
    prog_free(&e);
    chk_eq_u8("prog_free: zeroed result register", 0, e.r);
    chk_eq_u8("prog_free: zeroed counter register", 0, e.c);
    chk_eq_list("prog_free: empty listing", NULL, e.head);
    chk_eq_list("prog_free: empty listing", NULL, e.tail);

    // appended rule must succeed, it only returns zero on malloc failure
    r.u8 = 0;
    prog_init(&e);
    rv = prog_append(&e, &r);
    assert(rv);
    chk_ne_list("prog_append: non-empty listing", NULL, e.head);
    chk_ne_list("prog_append: non-empty listing", NULL, e.tail);

    // count listing length, find tail and its rule
    assert(e.head);
    assert(e.tail);
    rv = help_inspect_listing(&e, &tp);
    chk_eq_i("prog_append: listing length", 1, rv);

    // helper cannot fail to find the tail, since length > 0
    assert(tp);
    // prog_append has to create its own container then copy rule_t
    // it can't just point to the input rule, that will be on the stack
    chk_ne_rulep("prog_append: tail pointer rule address", &r, &(tp->r));
    // test that again a different way
    r.cjmp.in = PI_CJMP;
    r.cjmp.pci = 1;
    chk_eq_u8("prog_append: rule not shadow-written", 0, tp->r.u8);
    prog_free(&e);


    // create a 3x3 to start doing some real checks with
    prog_init(&e);
    rv = grid_init(&g, 1);
    assert(rv);

    // check compiler on an empty grid
    compiler_init(&c, &e);
    rv = compiler_readinfo(&c, &g, 0, 0);
    assert(rv);
    draw(&g);
    list(&e);
    chk_eq_i("prog_length: compiled one instruction", 1, rv);
    chk_eq_u8("e[0] is RET (in=0)", PI_RET, c.e->head->r.ret.in);
    chk_eq_u8("e[0] is RET (op=0)", 0, c.e->head->r.ret.op);
    grid_free(&g);
    prog_free(&e);

    // reallocate, give birth to a known test grid
    prog_init(&e);
    grid_init(&g, 3);
    grid_write(&g, 0, 0, 1);
    grid_write(&g, 1, 1, 1);
    grid_write(&g, 2, 0, 1);
    draw(&g);
    compiler_init(&c, &e);
    rv = compiler_readinfo(&c, &g, 1, 1);
    assert(rv);
    rv = prog_length(&e);
    chk_eq_i("prog_length: compiled two instructions", 2, rv);
    chk_eq_u8("e[0] is CJMP", PI_CJMP, e.head->r.cjmp.in);
    chk_eq_u8("e[0] CJMP range", 1, e.head->r.cjmp.pci);
    chk_eq_u8("e[1] is RET (in=0)", PI_RET, e.head->next->r.ret.in);
    chk_eq_u8("e[1] is RET (op=0)", 0, e.head->next->r.ret.op);

    rv = life_execute_cell(&e, &g, 1, 1);
    assert(rv);
    chk_eq_u8("life_execute_cell: cell <1,1> dies", 0, e.r);
    prog_free(&e);

    // construct mock program and verify on same test grid
    prog_init(&e);
    r.loi.in = PI_LOI;
    r.loi.i = 2;
    r.loi.j = 2;
    prog_append(&e, &r);
    r.loi.in = PI_LOI;
    r.loi.i = 1;
    r.loi.j = 2;
    prog_append(&e, &r);
    r.cmp.in = PI_CMP;
    r.cmp.op = CMP_OP_EQ;
    r.cmp.k = 2;
    prog_append(&e, &r);
    list(&e);
    rv = prog_length(&e);
    chk_eq_i("prog_length: injected three instructions", 3, rv);
    rv = life_execute_cell(&e, &g, 1, 1);
    assert(rv);
    chk_eq_u8("life_execute_cell: counted cells <0,0> and <0,2> from <1,1>", 2, e.c);
    chk_eq_u8("life_execute_cell: CMP [Rc] eq $2 (true) sets Rr", 1, e.r);
    prog_free(&e);

    return 0;
}