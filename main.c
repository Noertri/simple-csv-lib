#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "include/csv.h"

int main() {
    char buffer[256];
    FILE *fp = fopen("people-100.csv",\
            "r");

    if (fp == NULL) {
        perror("Fail when open file!!!");
        exit(-1);
    }
    
    CSV_OPTS options;

    CSV reader = csv_reader(fp);

    for (int i = 0; i < reader.rows; i++) {
        printf("%s\t", reader.str2d[i][0]);
        printf("%s\t", reader.str2d[i][8]);
        printf("\n");
    }

    fclose(fp);
    return 0;
}


