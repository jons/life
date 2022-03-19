/**
 * self-programming game of life
 * 
 * program definitions
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "prog.h"

/**
 */
void prog_init (prog_t *e)
{
    e->r = 0;
    e->c = 0;
    e->head = NULL;
    e->tail = NULL;
}

/**
 */
void prog_free (prog_t *e)
{
    while (e->head)
    {
        e->tail = e->head->next;
        free(e->head);
        e->head = e->tail;
    }
    e->head = NULL;
    e->tail = NULL;
    e->r = 0;
    e->c = 0;
}

/**
 */
int prog_loi_offset (uint8_t ofst)
{
    switch (ofst)
    {
        case 0:
        case 3:
            return 0;
        case 1:
            return 1;
        case 2:
            return -1;
    }
    assert(0);
}

/**
 */
int prog_append (prog_t *e, const rule_t *r)
{
    list_t *n;
    n = (list_t *)malloc(sizeof(list_t));
    if (n == NULL)
        return 0;
    n->next = NULL;
    memcpy(&n->r, r, sizeof(rule_t));
    if (e->tail == NULL)
    {
        e->head = n;
        e->tail = n;
    }
    else
    {
        e->tail->next = n;
        e->tail = n;
    }
    return 1;
}