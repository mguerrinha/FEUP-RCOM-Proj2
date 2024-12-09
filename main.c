#include "include/download.h"

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Usage: ./download ftp://<user>:<password>@<host>/<file-path>\n");
        exit(-1);
    }

    struct URL url;
    memset(&url, 0, sizeof(url));

    if (parse_url(argv[1], &url)) {
        printf("Usage: ./download ftp://<user>:<password>@<host>/<file-path>\n");
        exit(-1);
    }



}