#ifndef UTILS_H
#define UTILS_H

#include "configurations.h"

int min(int a, int b);
int max(int a, int b);
int lowbit(int x);
char read();
void init_grid(Grid* grid);
void init_square(Square* square, char shape);

#endif