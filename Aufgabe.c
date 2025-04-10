#include <stdio.h>
#include <stdlib.h>

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
    screen[x][y] = 1;
}

void line(int x0, int y0, int x1, int y1, char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH])
{
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    while (1) {
        setPixel(y0, x0, screen);
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

    printf("Drawing Structure\n");
    rasterCircle(xMax/2, yMax/2, n, screen);
    
    FileError err = writeInFile(screen);
    
    if (err != FILE_SUCCESS) {
        printf("Error: %s\n", file_error(err));
    }
}
