#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define HOST "localhost"
#define PORT "2000"

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
        printf("getaddrinfo error\n");
        exit(1);
    }

    if((socket_fd = socket(connection_info->ai_family, 
        connection_info->ai_socktype, connection_info->ai_protocol)) == -1) {
        printf("socket error\n");
        exit(1);
    }

    if(connect(socket_fd, connection_info->ai_addr, 
        connection_info->ai_addrlen) == -1) {
        close(socket_fd);
        printf("connect error\n");
        exit(1);
    }
    freeaddrinfo(connection_info);
    return socket_fd;
}

int main(int argc, char *argv[]) {
    int socket_fd;
    int time_stamp;
    int frame_size;
    char *filename = argv[1];
    int num_bytes = 1;
    char input_buffer[100];
    char output_buffer[100000];
    FILE *input_csv = NULL;
    char *input_line = NULL;
    size_t line_length = 0;
    ssize_t sent;
    struct timespec initial_time, current_time;


    setpriority(PRIO_PROCESS, 0, -20);
    socket_fd = initialize_connection();

    int i;
    for(i = 0; i < 100000; i++) {
        output_buffer[i] = 'x';
    }

    input_csv = fopen(filename, "r");

    if(input_csv == NULL) {
        perror("fopen error");
        exit(1);
    }

    clock_gettime(CLOCK_REALTIME, &initial_time);

    while(fscanf(input_csv, "%d,%d,%s\n", &frame_size, &time_stamp, 
        input_buffer) != EOF) {
        clock_gettime(CLOCK_REALTIME, &current_time);
        printf("%d, %d\n", frame_size, time_stamp);
        //printf("Sleeping for %ld seconds\n", (time_stamp - 
            //((initial_time.tv_nsec - current_time.tv_nsec) * 1000000)));
        usleep((time_stamp * 1000) - ((current_time.tv_nsec - 
            initial_time.tv_nsec) / 1000) - 
            ((current_time.tv_sec - initial_time.tv_sec) * 1000000));
        sent = send(socket_fd, output_buffer, frame_size, MSG_DONTWAIT);
        //if(sent == -1)
        //    printf("errno %d\n", errno);
        //printf("Sent %zd characters\n", sent);
    }

    close(socket_fd);
    return 0;
}