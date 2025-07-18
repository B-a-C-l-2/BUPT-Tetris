#ifndef TETRIS_H
#define TETRIS_H

#include "configurations.h"
#include "utils.h"

void rotate(Square* square, int angle);
void drop(Grid* grid, Square* square, int position, int landing_height);
void erode(Grid* grid, int landing_height);

#endif