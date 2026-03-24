/*
Simple c library to read and parse csv file
*/


#include <stdio.h>


typedef struct __options {
    char delimiter;
    char quote;
    char escape;
} CSV_OPTS;


void csv_reader(FILE *csv_fp);

