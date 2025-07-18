#ifndef AGENT_H
#define AGENT_H

#include "configurations.h"
#include "utils.h"
#include "tetris.h"
#include "get_features.h"

Action choose(Grid* grid, char shape);
double rewards(Grid clone, Square* square, int position);

#endif