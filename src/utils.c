#include "utils.h"

int min(int a, int b) {
    return a < b ? a : b;
}
int max(int a, int b) {
    return a > b ? a : b;
}
int lowbit(int x) {
    return x & -x;
}
char read() {
    char ans = (char)getchar();
    while (ans == '\n' || ans == ' ') {
        ans = (char)getchar();
    }
    return ans;
}
void init_grid(Grid* grid) {
    memset(grid->data, 0, sizeof(grid->data));
    memset(grid->height, 0, sizeof(grid->height));
    memset(grid->cnts, 0, sizeof(grid->cnts));
    grid->max_height = 0;
    grid->scores = 0;
    grid->data[0][0] = grid->data[0][W + 1] = grid->data[H + 1][0] = grid->data[H + 1][W + 1] = -1;
    for (int i = 1; i <= W; i++) {
        grid->data[0][i] = grid->data[H + 1][i] = -1;
    }
    for (int i = 1; i <= H; i++) {
        grid->data[i][0] = grid->data[i][W + 1] = -1;
    }
    grid->height[0] = grid->height[W + 1] = H;
}
void init_square(Square* square, char shape) {
    // int color = rand() % COLOR + 1;
    int color = 1;
    if (shape == 'I') {
        square->w = 4, square->h = 1;
        square->data[1][1] = square->data[1][2] = square->data[1][3] = square->data[1][4] = color;
        square->bottom[1] = square->bottom[2] = square->bottom[3] = square->bottom[4] = 1;
    } else if (shape == 'T') {
        square->w = 3, square->h = 2;
        square->data[1][1] = square->data[1][3] = 0;
        square->data[1][2] = square->data[2][1] = square->data[2][2] = square->data[2][3] = color;
        square->bottom[1] = square->bottom[2] = square->bottom[3] = 2;
    } else if (shape == 'O') {
        square->w = 2, square->h = 2;
        square->data[1][1] = square->data[1][2] = square->data[2][1] = square->data[2][2] = color;
        square->bottom[1] = square->bottom[2] = 2;
    } else if (shape == 'J') {
        square->w = 3, square->h = 2;
        square->data[1][2] = square->data[1][3] = 0;
        square->data[1][1] = square->data[2][1] = square->data[2][2] = square->data[2][3] = color;
        square->bottom[1] = square->bottom[2] = square->bottom[3] = 2;
    } else if (shape == 'L') {
        square->w = 3, square->h = 2;
        square->data[1][1] = square->data[1][2] = 0;
        square->data[1][3] = square->data[2][1] = square->data[2][2] = square->data[2][3] = color;
        square->bottom[1] = square->bottom[2] = square->bottom[3] = 2;
    } else if (shape == 'S') {
        square->w = 3, square->h = 2;
        square->data[1][1] = square->data[2][3] = 0;
        square->data[1][2] = square->data[1][3] = square->data[2][1] = square->data[2][2] = color;
        square->bottom[1] = square->bottom[2] = 2, square->bottom[3] = 1;
    } else if (shape == 'Z') {
        square->w = 3, square->h = 2;
        square->data[1][3] = square->data[2][1] = 0;
        square->data[1][1] = square->data[1][2] = square->data[2][2] = square->data[2][3]= color;
        square->bottom[1] = 1, square->bottom[2] = square->bottom[3] = 2;
    }
}