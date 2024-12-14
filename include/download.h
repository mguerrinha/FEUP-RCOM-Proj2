#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include <string.h>

#define MAX_LENGTH 1024
#define TOTAL_TIMEOUT 5000000
#define RETRY_DELAY 100000

#define FTP_PORT 21

#define DEFAULT_USERNAME "anonymous"
#define DEFAULT_PASSWORD "anonymous"

#define LOGGED_IN_CODE 220
#define PASSWORD_CODE 331
#define LOGIN_SUCCESSFULL_CODE 230
#define PASSIVE_MODE_CODE 227
#define DATA_CONNECTION_ESTABLISHED_CODE 125
#define START_TRANSFER_CODE 150
#define TRANSFER_COMPLETED_CODE 226
#define CLOSE_DATA_CONNECTION_CODE 221



struct URL {
    char user[MAX_LENGTH];
    char password[MAX_LENGTH];
    char host[MAX_LENGTH];
    char path[MAX_LENGTH];
    char ip[MAX_LENGTH];
};

typedef enum {
    START,
    SINGLE_LINE,
    MULTIPLE_LINE,
    COMPLETE
} ResponseState;

int parse_url(char *input, struct URL *url);

int create_socket(char *ip, int port);

int read_ftp_response(int socketfd, char *response);

int authenticate_ftp_user(int socketfd, const char *user, const char *password);

int set_passive_mode(int socketfd, char *ip, int *port);

int transfer_request(int socketfd, const char *path);

int get_file(int socket_control, int socket_data, const char *path);

int close_connection(int socket_control, int socket_data);