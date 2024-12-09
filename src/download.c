#include "../include/download.h"

int parse_url(char *input, struct URL *url) {
    if (strncmp(input, "ftp://", 6) != 0) {
        printf("Invalid URL\n");
        return -1;
    }
    
}

