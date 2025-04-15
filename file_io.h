#pragma once
#include <stdio.h>

enum {
    SCREEN_SIDE_LENGTH = 10000
};

typedef enum {
    FILE_SUCCESS = 0,
    FILE_OPEN_ERROR,
    FILE_READ_ERROR,
    FILE_WRITE_ERROR,
    FILE_CLOSE_ERROR,
    FILE_ERR_UNKNOWN,
} FileError;

const char* file_error(FileError err);

FileError writeInFile(char* screen);
