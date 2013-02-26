#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define HOST "localhost"
#define PORT "2000"
#define MAX_RECV_SIZE 1000

int initialize_connection() {
    int socket_fd;
    struct addrinfo hints;
    struct addrinfo *connection_info;
    int status = 0;
    
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(HOST, PORT, &hints, &connection_info)) != 0) {
        fprintf(stderr, "getaddrinfo error");
        exit(1);
    }

    if((socket_fd = socket(connection_info->ai_family, connection_info->ai_socktype, connection_info->ai_protocol)) == -1) {
        perror("socket error");
        exit(1);
    }

    if(connect(socket_fd, connection_info->ai_addr, connection_info->ai_addrlen) == -1) {
        close(socket_fd);
        perror("connect error");
        exit(1);
    }
    freeaddrinfo(connection_info);
    return socket_fd;
}

int main(int argc, char *argv[]) {
    int socket_fd;
    int num_bytes = 1;
    char buffer[MAX_RECV_SIZE];
    struct addrinfo *connection_info;

    socket_fd = initialize_connection();

    while(num_bytes != 0) {
        if((num_bytes = recv(socket_fd, buffer, MAX_RECV_SIZE-1, 0)) == -1) {
            perror("recv error");
            exit(1);
        }

        buffer[num_bytes - 1] = '\0';

        printf("string is: %s", buffer);    
    }
    close(socket_fd);
    return 0;
}


