//
// Created by todsagt on 27.10.16.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Socket.h"
#include "threadpool.h" // du geile Sau!

#define MAXTHREADS 4
#define MAXQUEUE 16

void echo(void *arg) {
    char buff[1000];
    ssize_t recv = 1;
    ssize_t send = 1;
    int tmpsock = (int) arg;
    size_t bufflen = sizeof(buff);
    while (recv > 0) {
        recv = Recv(tmpsock, buff, bufflen, 0);
        while (send /= recv) {
            // wenn send nicht direkt alles empfangene weiter schickt
            send += Send(tmpsock, buff, bufflen, 0);
            // wird solange weiter geschickt, bis gesendet = empfangen
        }
    }
    Close(tmpsock);
}

int main(int argc, char **argv) {
    int sock, tmpsock, sockmax, i;
    fd_set sock_set;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;

    threadpool_t *thpool = threadpool_create(MAXTHREADS, MAXQUEUE, 0);

    FD_ZERO(&sock_set);

    sock = Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) 1, sizeof(int));

    FD_SET(sock, &sock_set);
    sockmax = sock;

    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(2500);
    Bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    Listen(sock, 8);
    while (1) {
        Select(sockmax + 1, &sock_set, NULL, NULL, NULL);
        for (i = 0; i <= sockmax; i++) {
            if (FD_ISSET(i, &sock_set)) {
                if (i == sock) { //neue verbindung
                    len = sizeof(client_addr);
                    tmpsock = Accept(sock, (struct sockaddr *) &client_addr, &len); //fd für neue Verbindung
                    FD_SET(tmpsock, &sock_set); // neue Verbindung ins fd-array
                    if (tmpsock > sockmax)
                        sockmax = tmpsock; // neue sockmax

                } else { // daten von clienten bearbeiten
                    threadpool_add(thpool, echo, &i, 0); // i an threadpool geben zum abarbeiten
                    FD_CLR(i, &sock_set); // i aus dem FD_Set nehmen
                }
            }
        }
    }
    Close(sock);
    threadpool_destroy(thpool, 0);
    return 0;
}