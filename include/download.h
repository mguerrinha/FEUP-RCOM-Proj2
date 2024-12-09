#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#define MAX_LENGTH 256

struct URL {
    char user[MAX_LENGTH];
    char password[MAX_LENGTH];
    char host[MAX_LENGTH];
    char resource[MAX_LENGTH];
    int port;
};

int parse_url(char *input, struct URL *url);