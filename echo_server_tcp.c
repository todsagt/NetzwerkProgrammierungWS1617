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

void *echo(void *arg) {
    char buff[1<<16];
    ssize_t recv;
    ssize_t sent;
    int tmpsock = *((int*) arg);
    free(arg);
    do {
        memset(buff, 0, sizeof(buff));
        recv = Recv(tmpsock, buff, sizeof(buff), 0);
        printf("Daten erhalten\n");
        sent = 0;
        while (sent < recv) {
            // wenn send nicht direkt alles empfangene weiter schickt
            sent += Send(tmpsock, buff+sent, (recv-sent), 0); // buff pointer um send verschieben
            // wird solange weiter geschickt, bis gesendet = empfangen
        }
        printf("Daten gesendet\n");
    } while (recv > 0);
    Close(tmpsock);
    printf("Verbindung geschlossen\n");
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
        pthread_create(&thread, NULL, echo, tmpsock); // i an thread geben zum abarbeiten
        pthread_detach(thread);
    }
    Close(sock);
    return 0;
}
