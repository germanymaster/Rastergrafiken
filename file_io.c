#include "file_io.h"

#include <stdlib.h>


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

FileError writeInFile(char* screen)
{
    FILE *fptr;
    // Create/Rewrite file
    printf("Creating file...\n");
    errno_t err = fopen_s(&fptr, "test.ppm", "w");
    if (err != 0 || fptr == NULL) {
        return FILE_OPEN_ERROR;
    }
    if (fprintf(fptr, "P3\n %d %d\n 1\n", SCREEN_SIDE_LENGTH, SCREEN_SIDE_LENGTH) < 0) {
        return FILE_WRITE_ERROR;
    }
    printf("Building content...\n");
    char *str = calloc((SCREEN_SIDE_LENGTH * SCREEN_SIDE_LENGTH) * 6 + SCREEN_SIDE_LENGTH + 1, sizeof(char));
    for(int k = 0; k < SCREEN_SIDE_LENGTH * SCREEN_SIDE_LENGTH; k++)
    {
        for (int j = 0; j < 6; j++)
        {
            if ( j % 2 == 0)
            {
                str[k*6 + j] = screen[k] + 48;
            } else
            {
                str[k*6 + j] = ' ';
            }
        }
        if (k % SCREEN_SIDE_LENGTH == 9999)
        {
            str[k*6 + 6] = '\n';
        }
    }
    str[(SCREEN_SIDE_LENGTH * SCREEN_SIDE_LENGTH) * 6 + SCREEN_SIDE_LENGTH] = '\0';
    // Write file
    printf("Writing file...\n");
    /*if (fprintf(fptr,"%s", str) < 0)
    {
        return FILE_WRITE_ERROR;
    }*/
    fwrite(str, sizeof(char), SCREEN_SIDE_LENGTH * SCREEN_SIDE_LENGTH * 6 + SCREEN_SIDE_LENGTH, fptr);
    if (fprintf(fptr, "\n") < 0) {
        return FILE_WRITE_ERROR;
    }
    // Close File
    if (fclose(fptr) != 0) {
        return FILE_CLOSE_ERROR;
    }
    printf("File successfully created.\n");
    return FILE_SUCCESS;
}
