life
====

<p>the self-programming game of life. or <i>splife</i>.</p>

<p>in conway's game of life, the rule set for each cell is fixed:</p>
<ol><li>any live cell with fewer than two live neighbours dies, as if caused by under-population.</li>
<li>any live cell with two or three live neighbours lives on to the next generation.</li>
<li>any live cell with more than three live neighbours dies, as if by overcrowding.</li>
<li>any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.</li></ol>

<p>in variations on conway's game, the rule set is parameterized.</p>

<p>in <i>splife</i>, as each cell is processed, the rules are expressed as a program
stored in the grid itself, relative to the position of that cell.

## Machine Description

<p>program read order is clockwise, starting at "top dead center" for the row that the
reader is currently on, spiraling outward to each successive ring of cells (bits).</p>

<p>![program read order](docs/read-order.png)</p>

<p>in this illustration and in the rest of this document the current cell is just called x.</p>

<p>cell x is encircled by three bytes of code: blue, green, and yellow, in that order.
the astute reader will note that each ring contains one more byte than the last.</p>

<p>the current maximum/minimum extents of the living cells in the grid define what cells
are processed and thus how many programs are run for a single generation. addressing within
a program is done relative to the cell being processed, so as to circumvent the issue of
renumbering memory addresses from a new "zero" whenever the grid expands/contracts.</p>

<p>the machine does not have a stack or heap. the grid state is self-evident, and it is
its own program code, but nothing else. i guess that might be interesting to look into,
with a bigger word size (you'll see below).</p>

## Registers

<p>the splife machine offers two registers to a program.</p>

<p>`Rr` : the result register. stores only one bit of information. initialized to zero
at the beginning of each program (each cell), and always written back to that cell when
NOP/STOP is executed.</p>

<p>`Rc` : the counter register. stores three bits of information. initialized to zero
at the beginning of each program. read/written by other instructions.</p>

## Instructions

<p>the first two bits of each instruction word indicate what will be executed, with the
exception of the NOP/STOP and unconditional JUMP instructions, which share the prefix bits
`00`. thus, there are five instructions.</p>

<p>masks of each instruction below describe which bits must be set to 1 or 0 and which are
variable.</p>

#### NOP/STOP

  `00000000`

  __program read stops. the program is over.__

  * the current value of the result register Rr is written to cell position x.
  * lone cells die: this is their first/only instruction read (consistent with conway)

#### JUMP

  `00` `kkkkkk`

  __jump the next k instructions and resume execution__

  * k is a 6-bit, unsigned integer greater than zero (if zero, that would be NOP/STOP)

#### CONDITIONAL JUMP

  `01` `kkkkkk`

  __jump the next k instructions if Rc is 1. resume read/execution there.__

  * `k` is a 6-bit, unsigned integer.

#### COMPARE

  `10` `fff` `kkk`

  __compare the counter register (LHS) to a constant value, k (RHS)__

  * f defines the comparison operator:
    * `100` equal
    * `010` less-than
    * `110` less-than-or-equal
    * `001` greater-than
    * `101` greater-than-or-equal
    * `000` false
    * `111` true
    * `011` not-equal
  * k is a 3-bit, unsigned integer
  * store the result to the result register, Rr
  * if true then store 1, otherwise 0

#### LOAD-INCREMENT

  `11 `00` `ii` `jj`

  __reads a neighbor of x at offset i,j__

  * if the cell at offset i,j from x is live, then add 1 to the counter register, Rc
  * i and j are 2-bit, _one's complement_ integers:
    * `00` zero
    * `01` one (e.g. right or down)
    * `10` negative one (e.g. left or up)
    * `11` negative zero. same as zero.
  * whenever 1 is added to Rc and it was already at its max value of seven, Rc overflows to zero.
  * there is no overflow flag.
