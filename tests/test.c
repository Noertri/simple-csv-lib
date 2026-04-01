#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "../include/csv.h"


int main(int argc, char **argv) {
    char buffer[256];
    FILE *fp = fopen(argv[1],\
            "r");

    if (fp == NULL) {
        perror("Failed when open file!!!");
        exit(-1);
    }
    
    CSVOpts options = {0};
    options.null_value = "NONE";
    options.buffer_size = 4096;

    CSV reader = csv_reader(fp, 50000, options);

    for (int i = 0; i < reader.rows; i++) {
        printf("%d ", i);
        CSVRow row = reader.records[i];
        for (int j = 0; j < row.len; j++) {
            printf("%s ", row.values[j]);
        }

        printf("\n");
    }
    
    csv_destroy(reader);
    fclose(fp);
    return 0;
}


