#include "get_features.h"

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