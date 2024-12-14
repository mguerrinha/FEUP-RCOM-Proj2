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
        printf("No user information\n");
        url->user[0] = '\0';
        url->password[0] = '\0';
    }

    char *slash_pos = strchr(input, '/');
    if (slash_pos != NULL) {
        strncpy(url->host, input, slash_pos - input);
        url->host[slash_pos - input] = '\0';

        strcpy(url->path, slash_pos);
    }
    else {
        strcpy(url->host, input);
        url->path[0] = '\0';
    }

    struct hostent *host;
    
    if (strlen(url->host) == 0) {
        printf("Invalid host\n");
        return -1;
    }
    
    if ((host = gethostbyname(url->host)) == NULL) {
        printf("Invalid host '%s'\n", url->host);
        return -1;
    }

    strcpy(url->ip, inet_ntoa(*((struct in_addr *) host->h_addr_list[0])));

    return 0;
}

int create_socket(char *ip, int port) {
    int socketfd;
    struct sockaddr_in server_addr;

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error creating socket");
        exit(-1);
    }

    if (connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(-1);
    }

    return socketfd;
}

int read_ftp_response(int socketfd, char *response) {
    int total_bytes_read = 0, bytes_read = 0;
    int response_code = 0;

    // Clear the response buffer
    memset(response, 0, MAX_LENGTH);

    while (1) {
        // Attempt to read data from the socket
        bytes_read = recv(socketfd, response + total_bytes_read, MAX_LENGTH - total_bytes_read - 1, 0);
        
        if (bytes_read > 0) {
            // Successfully read some data, reset elapsed time
            total_bytes_read += bytes_read;

            // Check if the response ends with a newline (indicating completion)
            if (response[total_bytes_read - 1] == '\n') {
                break;
            }
        } else if (bytes_read == 0) {
            // Connection closed by the server
            printf("Connection closed by server.\n");
            break;
        } else {
            // Handle other socket errors
            perror("recv failed");
            return -1;
        }
    }

    // Null-terminate the response buffer
    response[total_bytes_read] = '\0';

    // Check if data was received
    if (total_bytes_read > 0) {
        printf("Server Response: %s\n", response);

        // Parse the response code (first 3 digits)
        sscanf(response, "%d", &response_code);
    } else {
        printf("No response received from server.\n");
    }

    return response_code;
}


int authenticate_ftp_user(int socketfd, const char *user, const char *password) {
    char response[MAX_LENGTH];
    char command[MAX_LENGTH];

    printf("Authenticating user\n");

    if (user[0] == '\0' || user == NULL) {
        snprintf(command, MAX_LENGTH, "USER %s\r\n", DEFAULT_USERNAME);
        printf("Using default username\n");
    }
    else {
        snprintf(command, MAX_LENGTH, "USER %s\r\n", user);
    }

    if (write(socketfd, command, strlen(command)) < 0) {
        perror("Error sending username");
        return -1;
    }

    if (read_ftp_response(socketfd, response) != PASSWORD_CODE) {
        printf("Failed to send username\n");
        return -1;
    }

    if (password[0] == '\0'  || password == NULL) {
        snprintf(command, MAX_LENGTH, "PASS %s\r\n", DEFAULT_PASSWORD);
    }
    else {
        snprintf(command, MAX_LENGTH, "PASS %s\r\n", password);
    }

    if (write(socketfd, command, strlen(command)) < 0) {
        perror("Error sending password");
        return -1;
    }

    if (read_ftp_response(socketfd, response) != LOGIN_SUCCESSFULL_CODE) {
        printf("Failed to send password\n");
        return -1;
    }

    return 0;
}

int set_passive_mode(int socketfd, char *ip, int *port) {
    char response[MAX_LENGTH];
    char command[MAX_LENGTH];

    snprintf(command, MAX_LENGTH, "PASV\r\n");
    if (write(socketfd, command, strlen(command)) < 0) {
        perror("Error sending passive mode command");
        return -1;
    }

    if (read_ftp_response(socketfd, response) != PASSIVE_MODE_CODE) {
        printf("Failed to set passive mode\n");
        return -1;
    }

    int ip1, ip2, ip3, ip4, p1, p2;
    sscanf(response, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &ip1, &ip2, &ip3, &ip4, &p1, &p2);

    snprintf(ip, MAX_LENGTH, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
    *port = p1 * 256 + p2;

    return 0;
}

int transfer_request(int socketfd, const char *path) {
    char response[7 + strlen(path)];
    char command[MAX_LENGTH];

    sprintf(command, "RETR %s\r\n", path);

    if (write(socketfd, command, strlen(command)) < 0) {
        perror("Error sending transfer request");
        return -1;
    }

    int response_code = read_ftp_response(socketfd, response);

    if (response_code != START_TRANSFER_CODE && response_code != DATA_CONNECTION_ESTABLISHED_CODE) {
        printf("Failed to start transfer %d\n", response_code);
        return -1;
    }

    return 0;
}

int get_file(int socket_control, int socket_data, const char *path) {
    // Extract filename from the given path
    const char *last_slash = strrchr(path, '/');
    const char *filename = last_slash == NULL ? path : last_slash + 1;

    printf("Downloading file %s\n", filename);

    // Open the file for writing
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char buffer[MAX_LENGTH];
    int bytes_read;

    // Read data from the data socket and write to the file
    while ((bytes_read = read(socket_data, buffer, sizeof(buffer))) > 0) {
        if (fwrite(buffer, 1, bytes_read, file) != bytes_read) {
            perror("Error writing to file");
            fclose(file);
            return -1;
        }
    }

    if (bytes_read < 0) {  // Handle `read` error
        perror("Error reading from data socket");
        fclose(file);
        return -1;
    }

    // Close the file
    fclose(file);
    
    if (socket_data > 0) {
        if (close(socket_data) < 0) {
            perror("Failed to close data connection");
            return -1;
        }
    }

    // Check server response after transfer
    char server_response[MAX_LENGTH];
    int response_code = read_ftp_response(socket_control, server_response);
    if (response_code != TRANSFER_COMPLETED_CODE) {
        printf("File transfer not completed successfully. Server Response: %s\n", server_response);
        return -1;
    }

    printf("File %s downloaded successfully.\n", filename);
    return 0;
}


int close_connection(const int socket_control, const int socket_data) {
    char server_response[MAX_LENGTH];
    int response_code;

    if (socket_control > 0) {
        write(socket_control, "QUIT\r\n", 6);
        response_code = read_ftp_response(socket_control, server_response);
        if (response_code != 221) { // 221 indicates successful logout
            printf("Failed to close connection: %s\n", server_response);
            return -1;
        }
    }

    if (socket_control > 0) {
        if (close(socket_control) < 0) {
            perror("Failed to close control connection");
            return -1;
        }
    }

    return 0;
}