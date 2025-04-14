#include "draw.h"
#include "math_utils.h"

#define _USE_MATH_DEFINES

void setPixel(int x, int y, char* screen)
{
    if ((x >= SCREEN_SIDE_LENGTH) || (y >= SCREEN_SIDE_LENGTH) || (x < 0) || (y < 0)) {
        return;
    }
    if (screen[y * SCREEN_SIDE_LENGTH + x] == 1)
    {
        return;
    }
    screen[y * SCREEN_SIDE_LENGTH + x] = 1;
}

void line(int x0, int y0, int x1, int y1, char* screen)
{
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    while (1) {
        setPixel(x0, y0, screen);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void rasterCircle(int x0, int y0, int radius, char* screen)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    setPixel(x0, y0 + radius, screen);
    setPixel(x0, y0 - radius, screen);
    setPixel(x0 + radius, y0, screen);
    setPixel(x0 - radius, y0, screen);

    while(x < y)
    {
        if (f >= 0)
        {
            y -= 1;
            ddF_y += 2;
            f += ddF_y;
        }
        x += 1;
        ddF_x += 2;
        f += ddF_x + 1;

        setPixel(x0 + x, y0 + y, screen);
        setPixel(x0 - x, y0 + y, screen);
        setPixel(x0 + x, y0 - y, screen);
        setPixel(x0 - x, y0 - y, screen);
        setPixel(x0 + y, y0 + x, screen);
        setPixel(x0 - y, y0 + x, screen);
        setPixel(x0 + y, y0 - x, screen);
        setPixel(x0 - y, y0 - x, screen);
    }
}


void drawDiamond(int xMax, int yMax, int n, char* screen)
{
    int halfX = xMax / 2;
    int halfY = yMax / 2;
    if (n == 1) {
        line(0, halfY, xMax, halfY, screen);
        return;
    }
    n--;
    for (int i = 0; i <= n; i++)
    {
        int yPos = (i*yMax/n);
        line(0, halfY, halfX, yPos, screen);
        line(yMax, halfY, halfX, yPos, screen);
    }
}


void drawLinesInCircle(int xMax, int yMax, int n, char* screen)
{
    double pi_fraction = 2*M_PI / n;
    PositionStruct points[1000] = {0, 0};
    for (int i = 0; i < n; i++)
    {
        int pointX = cos(i*pi_fraction) * yMax/2 + SCREEN_SIDE_LENGTH/2;
        int pointY = sin(i*pi_fraction) * xMax/2 + SCREEN_SIDE_LENGTH/2;
        printf("Number = %f\n", pi_fraction);
        printf("Point X = %d\n", pointX);
        printf("Point Y = %d\n", pointY);
        points[i].x = pointX;
        points[i].y = pointY;
    }
    for (int i = 0; points[i].x != 0 || points[i].y != 0; i++)
    {
        for (int j = 0; points[j].x != 0 || points[j].y != 0; j++)
        {
            if (i != j)
            {
                line(points[i].x, points[i].y, points[j].x, points[j].y, screen);
            }
        }
    }
    rasterCircle(xMax/2, yMax/2, ((xMax < yMax) ? xMax : yMax)/2, screen);
}

void drawLinesQuarterCircle(int xMax, int yMax, int n, char* screen)
{
    n++;
    double pi_fraction = -0.5*M_PI / n;
    line(0, yMax, 0, 0, screen);
    line(0, yMax, xMax, yMax, screen);
    line(0, 0, xMax, 0, screen);
    line(xMax, yMax, xMax, 0, screen);
    for (int i = 0; i < n; i++)
    {
        int pointX = cos(i*pi_fraction) * yMax;
        int pointY = sin(i*pi_fraction) * xMax + SCREEN_SIDE_LENGTH;
        line(0, yMax, pointX, pointY, screen);
        line(pointX, pointY, xMax, 0, screen);
    }
    rasterCircle(0, yMax, SCREEN_SIDE_LENGTH, screen);
}


trianglePos recursiveTriangle(trianglePos point, int n, char* screen)
{
    int ytemp = point.point1.y;
    int xtemp = point.point1.x;
    point.point1.x = point.point1.x + (point.point2.x-point.point1.x)/4;
    point.point1.y = point.point3.y - (point.point3.y - ytemp)/2;
    printf("HERE:%d %d\n", point.point3.y, ytemp);
    point.point2.x = xtemp + (point.point2.x - xtemp)*0.75;
    point.point2.y = point.point3.y - (point.point3.y - point.point2.y)/2;
    point.point3.y = ytemp;
    line(point.point1.x, point.point1.y, point.point3.x, point.point3.y, screen);
    line(point.point2.x, point.point2.y, point.point3.x, point.point3.y, screen);
    line(point.point1.x, point.point1.y,point.point2.x, point.point2.y, screen);
    if (n == 0)
    {
        return point;
    }
    recursiveTriangle(point, n-1, screen);
    return point;
}

trianglePos recursiveTripleTriangle(trianglePos point, int n, _Bool combo, char* screen)
{
    line(point.point1.x, point.point1.y, point.point3.x, point.point3.y, screen);
    line(point.point2.x, point.point2.y, point.point3.x, point.point3.y, screen);
    line(point.point1.x, point.point1.y,point.point2.x, point.point2.y, screen);
    if (combo) {
        recursiveTriangle(point, n, screen);
    }
    if (n == 0)
    {
        return point;
    }
    int ytemp = point.point1.y;
    int xtemp = point.point1.x;
    point.point1.x = round(point.point1.x + (point.point2.x-point.point1.x)/4);
    point.point1.y = round(point.point3.y - (point.point3.y - ytemp)/2);
    printf("HERE:%d %d\n", point.point3.y, ytemp);
    point.point2.x = round(xtemp + (point.point2.x - xtemp)*0.75);
    point.point2.y = round(point.point3.y - (point.point3.y - point.point2.y)/2);
    point.point3.y = point.point3.y;

    int xdiff = point.point2.x - point.point1.x;
    int ydiff = (point.point2.y - point.point3.y)*2;
    trianglePos triangle1 = point;
    triangle1.point1.x -= xdiff;
    triangle1.point2.x -= xdiff;
    triangle1.point3.x -= xdiff;
    recursiveTripleTriangle(triangle1, n-1, combo, screen);
    trianglePos triangle2 = point;
    triangle2.point1.x += xdiff;
    triangle2.point2.x += xdiff;
    triangle2.point3.x += xdiff;
    recursiveTripleTriangle(triangle2, n-1, combo, screen);
    trianglePos triangle3 = point;
    triangle3.point1.y += ydiff;
    triangle3.point2.y += ydiff;
    triangle3.point3.y += ydiff;
    recursiveTripleTriangle(triangle3, n-1, combo, screen);
    return point;
}
