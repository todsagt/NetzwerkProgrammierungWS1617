//
// Created by todsagt on 27.10.16.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Socket.h"

int main(int argc, char **argv)
{
    int sock, tmpsock, sockmax, i;
    fd_set sock_set;
    struct sockaddr_in server_addr, client_addr;
    char buff[1000];
    socklen_t len;
    ssize_t size;

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
    printf("Listen\n");
    while (1) {
        Select(sockmax+1,&sock_set, NULL, NULL, NULL);
        printf("Select\n");
        for(i=0;i <= sockmax;i++) {
            if (FD_ISSET(i,&sock_set)) {
                if(i == sock) { //neue verbindung
                    printf("Neue Verbindung\n");
                    len = sizeof(client_addr);
                    tmpsock = Accept(sock, (struct sockaddr *) &client_addr, &len); //fd für neue Verbindung
                    FD_SET(tmpsock,&sock_set); // neue Verbindung ins fd-array
                    if (tmpsock > sockmax)
                        sockmax = tmpsock; // neue sockmax

                } else { // daten von clienten bearbeiten
                    while( size > 0) {                          // schmeiß weg
                        size = Recv(i, buff, sizeof(buff), 0);  // lass liegen, tritt sich fest
                        printf("Daten wegschmeißen\n");
                    }
                    Close(i);
                    FD_CLR(i,&sock_set);
                }
            }
        }
    }
    Close(sock);
    return 0;
}
