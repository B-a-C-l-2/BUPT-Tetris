#include "tetris.h"

void rotate(Square* square, int angle) {
    Square clone;
    while (angle--) {
        clone = *square;
        for (int i = 1; i <= square->h; i++) {
            for (int j = 1; j <= square->w; j++) {
                clone.data[j][square->h - i + 1] = square->data[i][j];
            }
        }
        clone.w = square->h;
        clone.h = square->w;
        *square = clone;
    }
    for (int j = 1; j <= square->w; j++) {
        int bottom = square->h;
        while (square->data[bottom][j] == 0) {
            bottom--;
        }
        square->bottom[j] = bottom;
    }
}
void drop(Grid* grid, Square* square, int position, int landing_height) {
    for (int i = square->h; i >= 1; i--) {
        for (int j = 1; j <= square->w; j++) {
            if (square->data[i][j] != 0) {
                grid->data[landing_height + square->h - i][position + j - 1] = square->data[i][j];
                grid->cnts[landing_height + square->h - i]++;
            }
        }
    }
    grid->max_height = max(grid->max_height, landing_height + square->h - 1);
}
void erode(Grid* grid, int landing_height) {
    int eroded_rows = 0;
    int row = landing_height;
    while (row <= grid->max_height) {
        if (grid->cnts[row] == W) {
            for (int i = row + 1; i <= grid->max_height + 1; i++) {
                for (int j = 1; j <= W; j++) {
                    grid->data[i - 1][j] = grid->data[i][j];
                }
                grid->cnts[i - 1] = grid->cnts[i];
            }
            eroded_rows++;
            grid->max_height--;
        } else {
            row++;
        }
    }
    for (int j = 1; j <= W; j++) {
        for (int i = grid->max_height; i >= 0; i--) {
            if (grid->data[i][j] != 0) {
                grid->height[j] = i;
                break;
            }
        }
    }

    switch (eroded_rows) {
        case 1: grid->scores += 100; break;
        case 2: grid->scores += 300; break;
        case 3: grid->scores += 500; break;
        case 4: grid->scores += 800; break;
    }
}