#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    
    int sock, bytes_recieved;  
    char send_data[4096],recv_data[4096];
    struct hostent *host;
    struct sockaddr_in server_addr;  

    // Used local host
    host = gethostbyname("127.0.0.1");

    // socket creation
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;    
    server_addr.sin_port = htons(36000);  //***** used 36000 port for connection. *******
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8);

    // connection to server
    if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
    {
        perror("Connection error");
        exit(1);
    }

    printf("Connected to Server");

    //Loop to execute commands
    while(1) {
        puts("\nEnter Command here >>");
        fgets(send_data, sizeof(send_data), stdin);
        // handle commands and send them to server
        if (strcmp(send_data , "exit") != 0)
            send(sock,send_data,strlen(send_data), 0);
        // handle exit command
        else {
            send(sock,send_data,strlen(send_data), 0);  
            close(sock);
            break;
        }

        // print the response from server
        bytes_recieved=recv(sock,recv_data,1024,0);
        recv_data[bytes_recieved] = '\0';
        printf(">>%s " , recv_data);       
    }  
    return 0;
}
