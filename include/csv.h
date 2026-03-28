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

typedef struct __csv {
    int rows;
    int columns;
    CHAR3D_H records;
} CSV;

CSV csv_reader(FILE *csv_fp, CSV_OPTS options);

int csv_destroy(CSV csv);


