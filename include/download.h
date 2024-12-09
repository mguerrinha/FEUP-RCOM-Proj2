#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

struct URL {
    char *user;
    char *password;
    char *host;
    char *path;
    int port;
};

int parse_url(char *input, struct URL *url);