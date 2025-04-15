#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include "draw.h"
#include "file_io.h"
#include "math_utils.h"


int main()
{
    int n;
    printf("Gib den Wert von N ein: ");
    scanf_s("%d", &n);
    printf("%d\n", n);
    if (n < 0) {
        return -1;
    }

    char* screen = calloc(SCREEN_SIDE_LENGTH * SCREEN_SIDE_LENGTH, sizeof(char));

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
    _Bool combo = true;
    recursiveTripleTriangle(point, n, combo, screen);
    
    FileError err = writeInFile(screen);
    
    if (err != FILE_SUCCESS) {
        printf("Error: %s\n", file_error(err));
    }
    
    free(screen);

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
