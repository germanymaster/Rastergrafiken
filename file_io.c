#include "file_io.h"


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
    // Write file
    printf("Writing file...\n");
    if (fprintf(fptr, "P3\n %d %d\n 1\n", SCREEN_SIDE_LENGTH, SCREEN_SIDE_LENGTH) < 0) {
        return FILE_WRITE_ERROR;
    }
    for(int i = 0; i < SCREEN_SIDE_LENGTH; i++) {
        for(int j = 0; j < SCREEN_SIDE_LENGTH; j++) {
            if (fprintf(fptr,"%d %d %d ", screen[i * SCREEN_SIDE_LENGTH + j], screen[i * SCREEN_SIDE_LENGTH + j], screen[i * SCREEN_SIDE_LENGTH + j]) < 0) {
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
