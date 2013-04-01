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
#include <netinet/in.h>

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
    
    if((status = getaddrinfo(NULL, PORT, &hints, &connection_info)) != 0) {
        printf("getaddrinfo error\n");
        exit(1);
    }

    if((socket_fd = socket(connection_info->ai_family, 
        connection_info->ai_socktype, connection_info->ai_protocol)) == -1) {
        printf("socket error\n");
        exit(1);
    }

    if(bind(socket_fd, connection_info->ai_addr, connection_info->ai_addrlen) == -1) {
    	printf("bind error\n");
    	exit(1);
    }

    if(listen(socket_fd, 20) == -1) {
    	printf("listen error\n");
    	exit(1);
    }

    freeaddrinfo(connection_info);
    return socket_fd;
}

int main(int argc, char *argv[]) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int socket_fd;
    int time_stamp;
    int num_bytes = 1;
    char output_buffer[100000];
    struct timespec initial_time, current_time;
    int new_fd;
    int recv_len = 1;
    double current_minus_initial = 0;

    setpriority(PRIO_PROCESS, 0, -20);
    socket_fd = initialize_connection();

    addr_size = sizeof their_addr;

    new_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);
    clock_gettime(CLOCK_REALTIME, &initial_time);

    while(recv_len != 0) {
    	recv_len = recv(new_fd, &output_buffer, 100000, 0);
	    clock_gettime(CLOCK_REALTIME, &current_time);
	    current_minus_initial = (current_time.tv_sec - initial_time.tv_sec) * 1000.0;
	   	current_minus_initial += (current_time.tv_nsec - initial_time.tv_nsec) / 1000000.0;
	    printf("%d at %f\n", recv_len, current_minus_initial);
	}

    close(socket_fd);
    return 0;
}