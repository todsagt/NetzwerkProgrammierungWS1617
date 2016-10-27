//
// Created by todsagt on 27.10.16.
//
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zconf.h>
#include "Socket.h"

int main(int argc, char **argv)
{
    int sock, connfd;
    fd_set sock_set;
    struct sockaddr_in server_addr;
    char buff[1000];
    time_t ticks;
    ssize_t size;

    FD_ZERO(&sock_set);

    sock = Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) 1, sizeof(int));

    FD_SET(0, &sock_set);
    FD_SET(sock, &sock_set);

    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(2500); //well kown port 13, 2500 for execution without root
    Bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    Listen(sock, 8);
    while (1) {
        connfd = Accept(sock, (struct sockaddr *) NULL, NULL);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	write(connfd, buff, strlen(buff));
	shutdown(connfd, SHUT_WR);
	size = Read(sock, buff, sizeof(buff));
	// für connfd so lange read machen (schleife) bis read == 0
	if (size == 0)
	    shutdown(connfd, SHUT_RD);
        Close(connfd);
    }
    Close(sock);
    return 0;
}
