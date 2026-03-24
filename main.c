#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "csv.h"

int main() {
    char buffer[256];
    FILE *fp = fopen("people-100.csv",\
            "r");

    if (fp == NULL) {
        perror("Fail when open file!!!");
        exit(-1);
    }
    
    CSV_OPTS options;

    csv_reader(fp);
    fclose(fp);
    return 0;
}


