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
    screen[x][y] = 1;
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

int main()
{
    char screen[SCREEN_SIDE_LENGTH][SCREEN_SIDE_LENGTH] = {0};
    
    printf("Drawing Structure\n");
    line(0, 999, 999, 0, screen);
    line(0, 0, 999, 999, screen);
    
    FileError err = writeInFile(screen);
    
    if (err != FILE_SUCCESS) {
        printf("Error: %s\n", file_error(err));
    }
}
