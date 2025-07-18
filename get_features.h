#ifndef GET_FEATURES_H
#define GET_FEATURES_H

#include "configurations.h"
#include "utils.h"
#include "tetris.h"

int get_landing_height(Grid* grid, Square* square, int position);
int get_eroded_piece_cells(Grid* grid, Square* square, int landing_height);
int get_row_transitions(Grid* grid);
int get_column_transitions(Grid* grid);
int get_cumulative_wells(Grid* grid);
Hole_Info get_hole_info(Grid* grid);

#endif