#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define H 20    // 网格高度
#define W 10    // 网格宽度
#define LIMIT 16    // 死亡线
#define COLOR 9 // 颜色数
#define INF 1e9 // 无穷大

extern const double w[];    // 计算rewards的权重

typedef struct Grid {
    int data[H + 2][W + 2]; // data中-1表示墙，0表示空位，其他正数表示被填充
    int height[W + 2];  // 每列最高的被填充的格子的高度
    int cnts[H + 2];    // 每行被填充的格子的个数
    int max_height; // 整体的最大高度
    int scores; // 当前的得分
} Grid;  // 网格的结构
typedef struct Square {
    int w, h;   // 方块的宽度和高度
    int data[5][5]; // 和Grid的data类似，表示方块的形状
    int bottom[5];  // 每一列的最下方的被填充的格子的索引
} Square;  // 方块的结构
typedef struct Hole_Info {
    int holes;  // 空洞数
    int hole_depth; // 空洞的深度
    int rows_with_holes;    // 有空洞的行的数量
} Hole_Info;  // 收集有关hole的信息的结构
typedef struct Action {
    int angle;  // 角度
    int position;   // 下落的位置
} Action;  // 决策的结构体

#endif