#pragma once
#include "file_io.h"

typedef struct PosStruct
{
    int x;
    int y;
} PositionStruct;

typedef struct triangle_pos_struct
{
    PositionStruct point1;
    PositionStruct point2;
    PositionStruct point3;
} trianglePos;

void setPixel(int x, int y, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH]);
void line(int x0, int y0, int x1, int y1, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH]);
trianglePos recursiveTriangle(trianglePos point, int n, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH]);
trianglePos recursiveTripleTriangle(trianglePos point, int n, _Bool combo, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH]);
