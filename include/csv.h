/*
Simple c library to read and parse csv file
*/


#include <stdio.h>


typedef struct __options {
    char delimiter;
    char quote;
    char escape;
} CSV_OPTS;

typedef char** CHAR2D_H ;

typedef char*** CHAR3D_H;

typedef struct csv_result {
    int rows;
    int columns;
    CHAR3D_H str2d;
} CSV;

CSV csv_reader(FILE *csv_fp);

int csv_destroy(CSV csv);


