//
// Created by todsagt on 27.10.16.
//
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
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

    struct sockaddr_in server_addr;
    char buff[1000];
    time_t ticks;
    sock = Socket(AF_INET, SOCK_STREAM, 0);

    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(13); //well kown post

    Bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

    Listen(sock, 8);
    for (;;) {
        connfd = Accept(sock, (struct sockaddr *) NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        close(connfd);
    }
}
