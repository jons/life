/**
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
typedef struct page page_t;
typedef struct ofst ofst_t;

struct grid
{
  uint32_t d;
  uint8_t *m;
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
    uint8_t f   : 1;
    uint8_t op  : 2;
    uint8_t x   : 3;
    uint8_t z   : 2; // not used
  } bm;
};

struct page
{
  rule_t  r;
  uint8_t n;
};


void wcell(grid_t *g, uint32_t x, uint32_t y, uint8_t v);

uint8_t rcell(grid_t *g, uint32_t x, uint32_t y);

void info(page_t *p, grid_t *g, uint32_t x, uint32_t y);

void next_step(grid_t *n, grid_t *g, uint32_t x, uint32_t y);

void next_grid(grid_t *n, grid_t *g);

#endif
