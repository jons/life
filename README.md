life
====

<p>the self-programming game of life. or <i>splife</i>.</p>

<p>in conway's game of life, the rule set for each cell is fixed.</p>

<p>in variations on conway's game, the rule set is parameterized.</p>

<p>in <i>splife</i>, there is one rule per cell, and it is the program which is
read from the neighboring cells by taking each to be one bit of code/data.</p>

<p>the cell program read order is clockwise, starting at the top-left diagonal.</p>

<p>program structure is not fully general.</p>

<pre>
  struct {
    uint8_t f   : 1;
    uint8_t op  : 2;
    uint8_t x   : 3;
    uint8_t z   : 2; // not used
  } mask;
</pre>

<p>it consists of: the future state, one bit, to live or to die in the next
grid; an operator, two bits, GT, GTE, LT or LTE, the comparator used between the
number of neighbors counted in total and the program's only variable; and x,
three bits in this case, the program variable.</p>

<p>naturally you can see a bias in x in the current implementation. it will
always be less than n, and it will always come from the same neighbors, so a
lifeform cannot move in one general direction (downward, in this case).</p>

<p>this is the interesting part: to remove the bias.</p>


hypothesis a
------------

<p>the program should be read not per cell, but from the grid in its entirety.</p>

<p>beginning with the top-most, left-most live cell, and ending with the bottom-
most, right-most live cell, read all bits in the demarcated sub-grid. ignore the
first marker cell because it is always 1. either ignore the last cell, or perhaps
pad out the program with zeroes in some fashion.</p>

<p>interpret the state as a sequence of rules, one after another, possibly of a
more expressive structure than the currently designed. apply a rule to each cell,
including the dead cells outside where the program was read from (that point is
important), by treating the rule set as a circular queue.</p>
