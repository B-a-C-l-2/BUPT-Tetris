#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define H 20    // 网格高度
#define W 10    // 网格宽度
#define LIMIT 16    // 死亡线

#define COLOR 9 // 颜色数

#define INF 1e9 // 无穷大

const double w[] = {-12.63, +6.60, -9.22, -19.77, -10.49, -13.08, -1.61, -24.04};   // 计算rewards的权重

typedef struct Grid Grid;
typedef struct Square Square;
typedef struct Action Action;
typedef struct Hole_Info Hole_Info;

int min(int a, int b);
int max(int a, int b);
int lowbit(int x);
char read();
void init_grid(Grid* grid);
void init_square(Square* square, char shape);

void rotate(Square* square, int angle);
void drop(Grid* grid, Square* square, int position, int landing_height);
void erode(Grid* grid, int landing_height);

Action choose(Grid* grid, char shape);
double rewards(Grid clone, Square* square, int position);

int get_landing_height(Grid* grid, Square* square, int position);
int get_eroded_piece_cells(Grid* grid, Square* square, int landing_height);
int get_row_transitions(Grid* grid);
int get_column_transitions(Grid* grid);
int get_cumulative_wells(Grid* grid);
Hole_Info get_hole_info(Grid* grid);

void solve();

int main() {
    // srand((unsigned)time(NULL));

    solve();

    return 0;
}

struct Grid {
    int data[H + 2][W + 2]; // data中-1表示墙，0表示空位，其他正数表示被填充
    int height[W + 2];  // 每列最高的被填充的格子的高度
    int cnts[H + 2];    // 每行被填充的格子的个数
    int max_height; // 整体的最大高度
    int scores; // 当前的得分
};  // 网格的结构
struct Square {
    int w, h;   // 方块的宽度和高度
    int data[5][5]; // 和Grid的data类似，表示方块的形状
    int bottom[5];  // 每一列的最下方的被填充的格子的索引
};  // 方块的结构
struct Hole_Info {
    int holes;  // 空洞数
    int hole_depth; // 空洞的深度
    int rows_with_holes;    // 有空洞的行的数量
};  // 收集有关hole的信息的结构
struct Action {
    int angle;  // 角度
    int position;   // 下落的位置
};  // 决策的结构体

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

Action choose(Grid* grid, char shape) {
    Action ans;
    int best_angle = 0, best_position = 1;
    double best = -INF;

    Square square;
    init_square(&square, shape);

    int angle_limit = 4;
    if (shape == 'O') {
        angle_limit = 1;
    } else if (shape == 'I' || shape == 'S' || shape == 'Z') {
        angle_limit = 2;
    }

    for (int angle = 0; angle < angle_limit; angle++) {
        double scores = 0;
        rotate(&square, angle);
        for (int position = 1; position <= W - square.w + 1; position++) {
            scores = rewards(*grid, &square, position);
            if (scores > best) {
                best_angle = angle;
                best_position = position;
                best = scores;
            }
        }
        rotate(&square, 4 - angle);
    }

    ans.angle = best_angle;
    ans.position = best_position;

    return ans;
}
double rewards(Grid clone, Square* square, int position) {
    double ans = 0;

    int landing_height = get_landing_height(&clone, square, position);
    drop(&clone, square, position, landing_height);

    int eroded_piece_cells = get_eroded_piece_cells(&clone, square, landing_height);
    erode(&clone, landing_height);

    if (clone.max_height >= LIMIT) {
        return -INF;
    }

    int row_transitions = get_row_transitions(&clone);
    int column_transitions = get_column_transitions(&clone);
    int cumulative_wells = get_cumulative_wells(&clone);
    Hole_Info hole_info = get_hole_info(&clone);

    ans += w[0] * landing_height;
    ans += w[1] * eroded_piece_cells;
    ans += w[2] * row_transitions;
    ans += w[3] * column_transitions;
    ans += w[4] * cumulative_wells;
    ans += w[5] * hole_info.holes;
    ans += w[6] * hole_info.hole_depth;
    ans += w[7] * hole_info.rows_with_holes;

    return ans;
}

int get_landing_height(Grid* grid, Square* square, int position) {
    int landing_height = -1;
    for (int j = 1; j <= square->w; j++) {
        landing_height = max(landing_height, grid->height[position + j - 1] + square->bottom[j] - square->h + 1);
    }
    return landing_height;
}
int get_eroded_piece_cells(Grid* grid, Square* square, int landing_height) {
    int rows = 0;
    int cells = 0;
    for (int i = 1; i <= square->h; i++) {
        if (grid->cnts[landing_height + square->h - i] == W) {
            rows++;
            for (int j = 1; j <= square->w; j++) {
                if (square->data[i][j] != 0) {
                    cells++;
                }
            }
        }
    }
    return rows * cells;
}
int get_row_transitions(Grid* grid) {
    int ans = 0;
    for (int i = 1; i <= grid->max_height; i++) {
        for (int j = 2; j <= W; j++) {
            if ((!grid->data[i][j]) != (!grid->data[i][j - 1])) {
                ans++;
            }
        }
    }
    return ans;
}
int get_column_transitions(Grid* grid) {
    int ans = 0;
    for (int j = 1; j <= W; j++) {
        for (int i = 2; i <= grid->height[j] + 1; i++) {
            if ((!grid->data[i][j]) != (!grid->data[i - 1][j])) {
                ans++;
            }
        }
    }
    return ans;
}
int get_cumulative_wells(Grid* grid) {
    int ans = 0;
    for (int j = 1; j <= W; j++) {
        int well = 0;
        for (int i = min(grid->height[j - 1], grid->height[j + 1]); i >= 1; i--) {
            if (grid->data[i][j] == 0 && grid->data[i][j - 1] != 0 && grid->data[i][j + 1] != 0) {
                well++;
            } else {
                for (int k = 1; k <= well; k++) {
                    ans += k;
                }
                well = 0;
            }
        }
        if (well != 0) {
            for (int k = 1; k <= well; k++) {
                ans += k;
            }
        }
    }
    return ans;
}
Hole_Info get_hole_info(Grid* grid) {
    Hole_Info ans;
    int holes = 0, hole_depth = 0, rows_with_holes = 0;
    int rows_with_holes_binary = 0;

    for (int j = 1; j <= W; j++) {
        for (int i = grid->height[j]; i >= 1; i--) {
            if (grid->data[i][j] == 0) {
                holes++;
                hole_depth += grid->height[j] - i;
                rows_with_holes_binary |= 1 << i;
            }
        }
    }

    while (rows_with_holes_binary != 0) {
        rows_with_holes++;
        rows_with_holes_binary -= lowbit(rows_with_holes_binary);
    }

    ans.holes = holes;
    ans.hole_depth = hole_depth;
    ans.rows_with_holes = rows_with_holes;

    return ans;
}

void solve() {
    Grid grid;
    init_grid(&grid);

    Square square;
    char cur = read();

    Action action;
    int angle, position;

    while (cur != 'X') {
        char next = read();
        if (next == 'E' || grid.max_height >= LIMIT) {
            break;
        }

        action = choose(&grid, cur);
        angle = action.angle;
        position = action.position;

        init_square(&square, cur);
        rotate(&square, angle);

        int landing_height = get_landing_height(&grid, &square, position);
        drop(&grid, &square, position, landing_height);
        erode(&grid, landing_height);

        printf("%d %d\n", angle, position - 1);
        printf("%d\n", grid.scores);

        fflush(stdout);

        cur = next;
    }
}