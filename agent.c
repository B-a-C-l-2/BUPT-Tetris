#include "agent.h"

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