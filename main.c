#include "configurations.h"
#include "utils.h"
#include "tetris.h"
#include "get_features.h"
#include "agent.h"

const double w[] = {-13.30, -14.25, -5.03, -26.76, -15.57, -16.47, -6.33, -23.39};

int main() {
    // srand((unsigned)time(NULL));

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

    return 0;
}