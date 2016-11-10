//
// Created by todsagt on 27.10.16.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Socket.h"

#define MAXCLIENTS 10

int main(int argc, char **argv)
{
    int sock, tmpsock, i;
    int on = 1;
    fd_set sock_set;
    struct sockaddr_in server_addr;
    char buff[1000];
    ssize_t size;
    sock = Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &on, sizeof(int));
    

    int clientarray[MAXCLIENTS];
    for(i=0;i < MAXCLIENTS;i++) {
	clientarray[i]= -1;
    }
    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(2500);
    Bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    Listen(sock, 8);
    while (1) {
	FD_ZERO(&sock_set);
	FD_SET(sock, &sock_set);
	int max = sock;
	for(i=0;i < MAXCLIENTS;i++){
	    if(clientarray[i] == -1) {
		continue;
	    } else {
		if(max<clientarray[i]) {
		    max = clientarray[i]; // max für Select bestimmen
		}
		FD_SET(clientarray[i],&sock_set);
	    }
	}
        Select(max+1,&sock_set, NULL, NULL, NULL);
	if(FD_ISSET(sock,&sock_set)) {
	    tmpsock = Accept(sock, NULL, NULL);
	    for(i=0;i < MAXCLIENTS;i++){
	    	if(clientarray[i] == -1) {
		    printf("Neue Verbindung\n");
	    	    clientarray[i] = tmpsock;
		    break;
		}
	    }
	    if(i == MAXCLIENTS)
	    Close(tmpsock);
	}
	for(i=0;i < MAXCLIENTS;i++) {
	    if(clientarray[i] == -1) {
		break;
	    } else {
            	if (FD_ISSET(clientarray[i],&sock_set)) { // daten von clienten bearbeiten
                    size = Recv(clientarray[i], buff, sizeof(buff), 0);
		    if(size == 0) {
		    	Close(clientarray[i]);
		    	printf("Verbindung geschlossen\n");
		    	clientarray[i] = -1;
		    } else {
		    	printf("Daten wegwerfen: %.*s",(int)size,buff);
                    }
		}
            }
        }
    }
    Close(sock);
    return 0;
}
