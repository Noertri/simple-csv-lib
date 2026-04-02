/*
Simple c library to read and parse csv file
*/


#include <stdio.h>


#ifndef OPTIONS
#define OPTIONS
typedef struct __options {
    char delimiter;
    char quote;
    char escape;
    char *null_value;
    size_t buffer_size;
} CSVOpts;
#endif

typedef char** CHAR2D_H ;

typedef char*** CHAR3D_H;

typedef struct __csv_row {
    int len;
    CHAR2D_H values;
} CSVRow;

typedef CSVRow* RECORDS_H;

typedef struct __csv {
    int rows;
    RECORDS_H records;
} CSV;

CSV csv_reader(FILE *src, int max_row,\
        const CSVOpts csv_options);

int csv_destroy(CSV csv);


