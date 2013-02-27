#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>

#define HOST "localhost"
#define PORT "2000"

int last_time = 0;
int flag = 0;


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

void timer_handler(int signum) {
    flag = 0;
}

int main(int argc, char *argv[]) {
    int socket_fd;
    int time_stamp;
    int frame_size;
    char *filename = argv[1];
    int num_bytes = 1;
    char input_buffer[100];
    char output_buffer[100000];
    struct addrinfo *connection_info;
    FILE *input_csv = NULL;
    char *input_line = NULL;
    size_t line_length = 0;
    ssize_t sent;

    struct sigaction sa;
    struct itimerval timer;


    socket_fd = initialize_connection();

    for(int i = 0; i < 100000; i++) {
        output_buffer[i] = 'x';
    }

    input_csv = fopen(filename, "r");

    if(input_csv == NULL) {
        perror("fopen error");
        exit(1);
    }


    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);
    timer.it_value.tv_sec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    while(fscanf(input_csv, "%d,%d,%s\n", &frame_size, &time_stamp, 
        input_buffer) != EOF) {
        while(flag == 1);
        //printf("%d,%d\n", frame_size, time_stamp);
        sent = send(socket_fd, output_buffer, frame_size, 0);
        if(sent == -1)
            printf("errno %d\n", errno);
        printf("Sent %zd characters\n", sent);
        flag = 1;
        timer.it_value.tv_usec = (time_stamp - last_time) * 1000;
        last_time = time_stamp;
        setitimer(ITIMER_REAL, &timer, NULL);
    }

    close(socket_fd);
    return 0;
}