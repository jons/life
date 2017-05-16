life
====

<p>the self-programming game of life. or <i>splife</i>.</p>

<p>in conway's game of life, the rule set for each cell is fixed:</p>
<ol><li>any live cell with fewer than two live neighbours dies, as if caused by under-population.</li>
<li>any live cell with two or three live neighbours lives on to the next generation.</li>
<li>any live cell with more than three live neighbours dies, as if by overcrowding.</li>
<li>any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.</li></ol>

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

<p>beginning with the top-most, left-most cell, reading row at a time to the
right, and ending with the bottom-most row and the right-most cell, read all bits
in the demarcated sub-grid. the extents of the grid should be determined by the
furthest-placed live cell, not the total memory allocated for the grid.</p>

<p>interpret the state as a sequence of rules, one after another, possibly of a
more expressive structure than the currently designed. apply a rule to each cell,
including the dead cells outside where the program was read from (that point is
important), by treating the rule set as a circular queue: simply loop back
through the grid and loop through the program at the same time, applying the
current rule to the current grid location, until all grid locations have their
next state.</p>


hypothesis b
------------

<p>the program should read the grid as in hypothesis a, but only the positions of
the live cells.</p>

<p>scan grid memory and re-interpret the locations of the live cells as K-bit
natural numbers, which then can be taken for 8-bit instructions in the set. the
value of K is probably dependent on the extents of the memory available for the
grid.</p>
