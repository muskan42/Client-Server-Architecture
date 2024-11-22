#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

void trim_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int is_valid_priority(const char* str) {
    return strlen(str) == 1 && isdigit(str[0]) && (str[0] >= '1' && str[0] <= '3');
}

int main(int argc, char *argv[]) {
    int sock, bytes_received;
    char send_data[4096], recv_data[4096];
    struct hostent *host;
    struct sockaddr_in server_addr;

    // Resolve server address
    host = gethostbyname("127.0.0.1");
    if (host == NULL) {
        perror("Failed to resolve host");
        exit(1);
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(36000);
    server_addr.sin_addr = *((struct in_addr*)host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Connection to server failed");
        exit(1);
    }

    printf("Connected to server\n");

    // Main communication loop
    while (1) {
        printf("Enter Priority (1 = High, 2 = Medium, 3 = Low) and Command (format: PRIORITY:COMMAND):\n");
        fgets(send_data, sizeof(send_data), stdin);
        trim_newline(send_data);

        // Check for exit command
        if (strcmp(send_data, "exit") == 0) {
            send(sock, "2:EXIT", strlen("2:EXIT"), 0);  // Send exit command to server with medium priority
            printf("Closing connection...\n");
            close(sock);
            break;
        }

        // Validate input format (PRIORITY:COMMAND)
        char priority[2], command[4096];
        if (sscanf(send_data, "%1[0-9]:%[^\n]", priority, command) != 2 || !is_valid_priority(priority)) {
            printf("Invalid input format. Please use the format: PRIORITY:COMMAND (e.g., 1:HELP).\n");
            continue;
        }

        // Send validated data to server
        send(sock, send_data, strlen(send_data), 0);

        // Receive response from server
        bytes_received = recv(sock, recv_data, sizeof(recv_data) - 1, 0);
        if (bytes_received <= 0) {
            printf("Server disconnected or an error occurred. Exiting...\n");
            close(sock);
            break;
        }
        recv_data[bytes_received] = '\0';
        printf("Server response: %s\n", recv_data);
    }

    return 0;
}
