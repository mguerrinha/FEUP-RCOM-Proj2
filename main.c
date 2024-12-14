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

    // Print the extracted components
    printf("Username: %s\n", url.user);
    printf("Password: %s\n", url.password);
    printf("Host: %s\n", url.host);
    printf("Resource: %s\n", url.path);

    char response[MAX_LENGTH];
    int socket_control = create_socket(url.ip, FTP_PORT);

    if (socket_control < 0) {
        printf("Failed to connect to the FTP server\n");
        exit(-1);
    }

    int rpcd = read_ftp_response(socket_control, response);

    if (rpcd < 0) {
        printf("Failed to connect to the FTP server\n");
        return -1;
    }
    else if (rpcd != LOGGED_IN_CODE) {
        printf("Failed to connect to the FTP server\n");
        return -1;
    }

    printf("Connected to the FTP server\n");
    if (authenticate_ftp_user(socket_control, url.user, url.password) != 0) {
        printf("Failed to authenticate user\n");
        exit(-1);
    }

    printf("User authenticated\n");
    int data_port;
    char data_ip[MAX_LENGTH];

    printf("Setting passive mode\n");
    if (set_passive_mode(socket_control, data_ip, &data_port) != 0) {
        printf("Failed to set passive mode\n");
        exit(-1);
    }

    printf("Data IP: %s\n", data_ip);
    int socket_data = create_socket(data_ip, data_port);
    if (socket_data < 0) {
        printf("Failed to connect to the FTP server\n");
        exit(-1);
    }

    printf("Request to Download file\n");
    if (transfer_request(socket_control, url.path) != 0) {
        printf("Failed to transfer file\n");
        exit(-1);
    }

    if (get_file(socket_control, socket_data, url.path) != 0) {
        printf("Failed to download file\n");
        exit(-1);
    }

    if (close_connection(socket_control, socket_data) != 0) {
        printf("Failed to close connection\n");
        exit(-1);
    }
    
    return 0;
}