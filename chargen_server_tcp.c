//
// Created by todsagt on 27.10.16.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Socket.h"
#include "pthread.h"

void *chargen(void *arg) {
    char buff[1000];
    int tmpsock = *((int*) arg);
    free(arg);
    fd_set read_set;
    fd_set write_set;
    size_t recv = 0;
    size_t sen = 0;
    char data[54] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\0";
    while(1) {    
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_SET(tmpsock,&read_set);
	FD_SET(tmpsock,&write_set);

        Select(tmpsock+1,&read_set,&write_set,NULL,NULL);

        if(FD_ISSET(tmpsock,&read_set)) {
            recv = Recv(tmpsock, buff, sizeof(buff), 0); //ab ins nirvana
            printf("Recieved and thrown away %d bytes\n", (int) recv);
	    if (recv == 0)
	    break;
        }

        if(FD_ISSET(tmpsock,&write_set)) {
            if((sen = send(tmpsock, data, sizeof(data), 0)) <= 0) {
		break;
            }
        }
    }
    Close(tmpsock);
    return NULL;
}

int main(int argc, char **argv) {
    int sock, *tmpsock;
    int on = 1;
    struct sockaddr_in server_addr;
    pthread_t thread;
    sock = Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &on, sizeof(int));
    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(2500);
    Bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    Listen(sock, 8);
    while (1) {
	tmpsock = (int *)malloc(sizeof(int));
        *tmpsock = Accept(sock, NULL, NULL); //fd für neue Verbindung
        printf("Neue Verbindung\n");
        pthread_create(&thread, NULL, chargen, tmpsock); // i an thread geben zum abarbeiten
        pthread_detach(thread);
    }
    Close(sock);
    return 0;
}
