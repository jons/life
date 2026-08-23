/**
 * self-programming game of life
 * 
 * load grids from text files
 */
#ifndef LOAD_H
#define LOAD_H
#include "grid.h"

/**
 * load an N by N grid of '0' and '1' characters from the text file at PATH into grid G
 *
 * G must already be initialized to dimension N, the length of the first row
 * of the file. the file is one row of the grid per line, N characters per line
 * '0' cells are dead and '1' cells are live; any other character is an error
 * blank lines once the grid is complete are ignored
 * load_grid neither allocates nor frees G; on failure G is left in place,
 * possibly partially written, and the caller still frees it with grid_free
 * returns 1 on success, 0 on failure
 */
int load_grid (grid_t *g, const char *path);

#endif
