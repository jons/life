/**
 * self-programming life
 */
#ifndef LIFE_H
#define LIFE_H
#include <stdint.h>

#define OP_GT   (0)
#define OP_GTE  (1)
#define OP_LT   (2)
#define OP_LTE  (3)

typedef struct grid grid_t;
typedef union rule rule_t;
typedef struct ofst ofst_t;
typedef struct list list_t;
typedef struct prog prog_t;

struct grid
{
  struct
  {
    uint8_t t;  //  top
    uint8_t b;  //  bottom
    uint8_t l;  //  left
    uint8_t r;  //  right
  } e;          // extents of live cells
  uint32_t d;   // dimension of the grid
  uint32_t i;   // current iteration
  uint8_t *m;   // grid memory
};

struct ofst
{
  int8_t x;
  int8_t y;
};

union rule
{
  uint8_t u8;
  struct {
    uint8_t f   : 1;  // alive/dead register
    uint8_t op  : 2;  // opcode (see OP_* values)
    uint8_t x   : 3;  // operand
    uint8_t z   : 2;  // not used
  } bm;
};

struct list
{
  rule_t r;     // current instruction
  list_t *next; // container of next one
};

struct prog
{
  list_t *head, // start of listing
         *tail;
};



void wcell (grid_t *g, uint32_t x, uint32_t y, uint8_t v);

uint8_t rcell (grid_t *g, uint32_t x, uint32_t y);

uint8_t scan (grid_t *g, uint32_t x, uint32_t y);

void read_info (prog_t *e, grid_t *g);

void dump_info (prog_t *e);

void next_step (grid_t *n, grid_t *g, rule_t *r, uint32_t x, uint32_t y);

void next_grid (grid_t *n, grid_t *g);

int init_grid (grid_t *g, uint32_t d);

void free_grid (grid_t *g);

#endif
