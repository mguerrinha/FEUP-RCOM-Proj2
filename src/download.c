#include "../include/download.h"

// ftp://[username:password]@host/url-path

int parse_url(char *input, struct URL *url) {
    
    if (strncmp(input, "ftp://", 6) != 0) {
        printf("Invalid URL\n");
        return -1;
    }

    input += 6;

    char *at_pos = strchr(input, '@');
    if (at_pos != NULL) {
        const char *colon_pos = strchr(input, ':');
        if (colon_pos != NULL && colon_pos < at_pos) {
            strncpy(url->user, input, colon_pos - input);
            url->user[colon_pos - input] = '\0';

            strncpy(url->password, colon_pos + 1, at_pos - colon_pos - 1);
            url->password[at_pos - colon_pos - 1] = '\0';
        }
        else {
            strncpy(url->user, input, at_pos - input);
            url->user[at_pos - input] = '\0';
            url->password[0] = '\0';
        }

        input = at_pos + 1;
    }
    else {
        url->user[0] = '\0';
        url->password[0] = '\0';
    }

    char *slash_pos = strchr(input, '/');
    if (slash_pos != NULL) {
        strncpy(url->host, input, slash_pos - input);
        url->host[slash_pos - input] = '\0';

        strcpy(url->resource, slash_pos + 1);
    }
    else {
        strcpy(url->host, input);
        url->resource[0] = '\0';
    }

    return 0;
}

