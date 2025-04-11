#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <Windows.h> 

enum {
    SCREEN_SIDE_LENGTH = 1000
};

typedef enum {
    FILE_SUCCESS = 0,
    FILE_OPEN_ERROR,
    FILE_READ_ERROR,
    FILE_WRITE_ERROR,
    FILE_CLOSE_ERROR,
    FILE_ERR_UNKNOWN,
} FileError;

typedef struct posStruct
{
    int x;
    int y;
} PositionStruct;

const char* file_error(FileError err) {
    switch (err) {
    case FILE_SUCCESS:
        return "No error";
    case FILE_OPEN_ERROR:
        return "Could not open file";
    case FILE_WRITE_ERROR:
        return "Could not write to file";
    case FILE_READ_ERROR:
        return "Could not read from file";
    case FILE_CLOSE_ERROR:
        return "Could not close file";
    default:
        return "Unknown error";
    }
}

void setPixel(int x, int y, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
{
    if ((x >= SCREEN_SIDE_LENGTH) || (y >= SCREEN_SIDE_LENGTH) || (x < 0) || (y < 0)) {
        return;
    }
    if (screen[y][x] == 1)
    {
        return;
    }
    screen[y][x] = 1;
}

void line(int x0, int y0, int x1, int y1, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
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

void rasterCircle(int x0, int y0, int radius, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
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

FileError writeInFile(char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
{
    FILE *fptr;
    // Create/Rewrite file
    printf("Creating file...\n");
    errno_t err = fopen_s(&fptr, "test.ppm", "w");
    if (err != 0 || fptr == NULL) {
        return FILE_OPEN_ERROR;
    }
    // Write file
    printf("Writing file...\n");
    if (fprintf(fptr, "P3\n %d %d\n 1\n", SCREEN_SIDE_LENGTH, SCREEN_SIDE_LENGTH) < 0) {
        return FILE_WRITE_ERROR;
    }
    for(int i = 0; i < SCREEN_SIDE_LENGTH; i++) {
        for(int j = 0; j < SCREEN_SIDE_LENGTH; j++) {
            if (fprintf(fptr,"%d %d %d ", screen[i][j], screen[i][j], screen[i][j]) < 0) {
                return FILE_WRITE_ERROR;
            }
        }
        if (fprintf(fptr, "\n") < 0) {
            return FILE_WRITE_ERROR;
        }
    }
    // Close File
    if (fclose(fptr) != 0) {
        return FILE_CLOSE_ERROR;
    }
    printf("File successfully created.\n");
    return FILE_SUCCESS;
}

void drawDiamond(int xMax, int yMax, int n, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
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

void drawLinesInCircle(int xMax, int yMax, int n, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
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

void drawLinesQuarterCircle(int xMax, int yMax, int n, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
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

typedef struct triangle_pos_struct
{
    PositionStruct point1;
    PositionStruct point2;
    PositionStruct point3;
} trianglePos;

trianglePos recursiveTriangle(trianglePos point, int n, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
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

trianglePos recursiveTripleTriangle(trianglePos point, int n, _Bool combo, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
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

int main()
{
    int n;
    printf("Gib den Wert von N ein: ");
    scanf_s("%d", &n);
    printf("%d\n", n);
    if (n < 0) {
        return -1;
    }
    char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH] = {0};

    int xMax = SCREEN_SIDE_LENGTH - 1;
    int yMax = SCREEN_SIDE_LENGTH - 1;

    printf("Drawing Structure...\n");
    line(0, 0, xMax, 0, screen);
    int posX = cos(M_PI * 2/6) * xMax;
    int posY = sin(M_PI * 2/6) * yMax;
    line(0, 0, posX, posY, screen);
    posX = cos(M_PI * 4/6) * xMax + SCREEN_SIDE_LENGTH;
    posY = sin(M_PI * 4/6) * yMax;
    line(xMax, 0, posX, posY, screen);
    printf("%d\n%d\n", posX, posY);
    //
    int posX1 = cos(M_PI * 2/6) * xMax/2;
    int posY1 = sin(M_PI * 2/6) * yMax/2;
    line(posX1, posY1, xMax/2, 0, screen);
    int posX2 = cos(M_PI * 4/6) * xMax/2 + SCREEN_SIDE_LENGTH;
    int posY2 = sin(M_PI * 4/6) * yMax/2;
    line(posX2, posY2, xMax/2, 0, screen);
    
    line(posX1, posY1, posX2, posY2, screen);
    //
    trianglePos point;
    point.point1.x = posX1;
    point.point1.y = posY1;
    point.point2.x = posX2;
    point.point2.y = posY2;
    point.point3.x = xMax/2;
    point.point3.y = 0;
    //recursiveTriangle(point, n, screen);
    _Bool combo = false;
    recursiveTripleTriangle(point, n, combo, screen);
    
    FileError err = writeInFile(screen);
    
    if (err != FILE_SUCCESS) {
        printf("Error: %s\n", file_error(err));
    }
    
    char path[MAX_PATH];
    DWORD result = GetCurrentDirectoryA(MAX_PATH, path);
    
    if (result == 0) {
        printf("Fehler beim Abrufen des Pfads.\n");
    } else {
        printf("Vollständiger Pfad zur .exe:\n%s\n", path);
    }
}

void calcPos(xMax, yMax, n, screen)
{
    int posX = cos(M_PI * 2/6) * xMax;
    int posY = sin(M_PI * 2/6) * yMax;
    trianglePos point;
    point.point1.x = cos(M_PI * 2/6) * xMax/2;
    point.point1.y = sin(M_PI * 2/6) * yMax/2;
    point.point2.x = cos(M_PI * 4/6) * xMax*1.5;
    point.point2.y = sin(M_PI * 4/6) * yMax/2;
    point.point3.x = xMax/2;
    point.point3.y = sin(M_PI * 2/6) * yMax;
    recursiveTriangle(point, n, screen);
}
