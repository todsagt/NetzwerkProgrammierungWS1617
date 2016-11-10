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
#include "errno.h"

void *chargen(void *arg) {
    char buff[1000];
    int tmpsock = *((int*) arg);
    free(arg);
    fd_set read_set;
    fd_set write_set;
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_SET(tmpsock,&read_set);
    FD_SET(tmpsock,&write_set);
    size_t recv = 0;
    //size_t send = 0;
    char data[54] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\0";
    while(1) {
        Select(tmpsock+1,&read_set,&write_set,NULL,NULL);
        if(FD_ISSET(tmpsock,&write_set)) {
            if(/*send = */send(tmpsock, data, sizeof(data), 0) == -1) {
                //fprintf(stderr,"send:%s\n",strerror(errno));
            }
            //printf("Send %d bytes\n", (int) send);
        }
        if(FD_ISSET(tmpsock,&read_set)) {
            recv = Recv(tmpsock, buff, sizeof(buff), 0); //ab ins nirvana
            printf("Recieved and thrown away %d bytes\n", (int) recv);
        }
    }
    Close(tmpsock);
    return NULL;
}

int main(int argc, char **argv) {
    int sock, tmpsock/*, sockmax, i*/;
    //fd_set sock_set;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    pthread_t thread;

    //FD_ZERO(&sock_set);
    sock = Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, MSG_NOSIGNAL, (const void *) 1, sizeof(int));
    //SO_NOSIGPIPE auf bsd?

    //FD_SET(sock, &sock_set);
    //sockmax = sock;

    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(2500);
    Bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    Listen(sock, 8);
    printf("Listen\n");
    while (1) {
        //printf("Pre Select\n");
        //Select(sockmax + 1, &sock_set, NULL, NULL, NULL);
        //printf("Post Select\n");
        //for (i = 0; i <= sockmax; i++) {
        //if (FD_ISSET(i, &sock_set)) {
        //if (i == sock) { //neue verbindung
        len = sizeof(client_addr);
        tmpsock = Accept(sock, (struct sockaddr *) &client_addr, &len); //fd für neue Verbindung
        printf("Neue Verbindung\n");
        //    FD_SET(tmpsock, &sock_set); // neue Verbindung ins fd-array
        //    if (tmpsock > sockmax)
        //        sockmax = tmpsock; // neue sockmax

        //} else { // daten von clienten bearbeiten
        void *ptr = malloc(sizeof(int));
        //    *((int*)ptr) = i;
        *((int*)ptr) = tmpsock;
        pthread_create(&thread, NULL, chargen, ptr); // i an thread geben zum abarbeiten
        pthread_detach(thread);
        //    FD_CLR(i, &sock_set); // i aus dem FD_Set nehmen
        //}
        //}
        //}
    }
    Close(sock);
    return 0;
}