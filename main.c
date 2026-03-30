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
    options.null_value = "NONE";

    CSV reader = csv_reader(fp, 200, 4096, options);

    for (int i = 0; i < reader.rows; i++) {
        //printf("%d ", i+1);
        CSVRow row = reader.records[i];
        for (int j = 0; j < row.len; j++) {
            printf("%s ", row.values[j]);
        }

        printf("\n");
    }
    
    //csv_destroy(reader);
    fclose(fp);
    return 0;
}


