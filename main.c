#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "include/csv.h"


int main() {
    char buffer[256];
    FILE *fp = fopen("samples/people.csv",\
            "r");

    if (fp == NULL) {
        perror("Failed when open file!!!");
        exit(-1);
    }
    
    CSV_OPTS options = {0};

    CSV reader = csv_reader(fp, 256, options);

    for (int i = 0; i < reader.rows; i++) {
        for (int j = 0; j < reader.columns; j++) {
            printf("%s ", reader.records[i][j]);
        }
        printf("\n");
    }
    
    csv_destroy(reader);
    fclose(fp);
    return 0;
}


