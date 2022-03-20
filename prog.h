/**
 * self-programming game of life
 * 
 * program definitions
 */
#ifndef PROG_H
#define PROG_H
#include <stdint.h>

// program instructions
#define PI_RET  (0) // 00
#define PI_CJMP (1) // 01
#define PI_CMP  (2) // 10
#define PI_LOI  (3) // 11

// COMPARE operators
//   bit 0: r |= c > k
//   bit 1: r |= c < k
//   bit 2: r |= c == k
#define CMP_OP_F    (0) // 000
#define CMP_OP_GT   (1) // 001
#define CMP_OP_LT   (2) // 010
#define CMP_OP_NEQ  (3) // 011
#define CMP_OP_EQ   (4) // 100
#define CMP_OP_GTE  (5) // 101
#define CMP_OP_LTE  (6) // 110
#define CMP_OP_T    (7) // 111

typedef union rule rule_t;
typedef struct list list_t;
typedef struct prog prog_t;

union rule
{
  uint8_t u8;
  // RET and unconditional JUMP
  struct {
    uint8_t op  : 6;  // all operands
    uint8_t in  : 2;  // instruction 00
  } ret;
  // conditional JUMP
  struct {
    uint8_t pci : 6;  // program counter increment
    uint8_t in  : 2;  // instruction 01
  } cjmp;
  // COMPARE
  struct {
    uint8_t k   : 3;  // constant
    uint8_t op  : 3;  // comparison operator
    uint8_t in  : 2;  // instruction 10
  } cmp;
  // LOAD-INCREMENT
  struct {
    uint8_t j   : 2;  // col-offset
    uint8_t i   : 2;  // row-offset
    uint8_t u   : 2;  // ignored
    uint8_t in  : 2;  // instruction 11
  } loi;
};

struct list
{
  rule_t r;     // current instruction
  list_t *next; // container of next one
};

struct prog
{
  uint8_t r;    // result register, Rr
  uint8_t c;    // counter register, Rc
  list_t *head, // listing
         *tail;
};

struct compiler
{
    prog_t *e;  // the program being compiled
    uint8_t jr; // current jump range (distance to continue to require reading)
};


/**
 */
void prog_init (prog_t *e);

/**
 */
void prog_free (prog_t *e);

/**
 * translate unsigned int value of rule.loi.[i,j] to real signed int offset
 */
int prog_loi_offset (uint8_t ofst);

/**
 * add rule to the bottom of the listing in program e.
 */
int prog_append (prog_t *e, const rule_t *r);

#endif