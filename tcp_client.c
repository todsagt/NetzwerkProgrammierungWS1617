//
// Created by todsagt on 25.10.16.
//

#define _WITH_GETLINE

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "Socket.h"
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include "signal.h"

#define BUFFER_SIZE  (1<<16)
#define MESSAGE_SIZE (9216)

int sock;

void onClose(int dummy) {
    shutdown(sock,SHUT_WR);
    exit(0);
}

int main(int argc, char **argv) {
    char buf[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    int sock;
    ssize_t size;
    fd_set sock_set;
    fd_set read_set;
    //sock
    sock = Socket(AF_INET,SOCK_STREAM,0);
    FD_ZERO(&sock_set);
    FD_ZERO(&read_set);
    FD_SET(0,&sock_set);
    FD_SET(sock,&sock_set);

    signal(SIGINT, onClose);
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&buf, 0, sizeof(buf));
    server_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
    server_addr.sin_len = sizeof(struct sockaddr_in);
#endif
    if (argc < 2) { // get port
	fprintf(stderr, "No Port given..\n");
	exit(-1);
    }
    server_addr.sin_port = htons(atoi(argv[2]));
    if (argc < 1) { // get adress
	fprintf(stderr, "No Adress given..\n");
	exit(-1);
    }
    if ((server_addr.sin_addr.s_addr = (in_addr_t) inet_addr(argv[1])) == INADDR_NONE) {
        fprintf(stderr, "Invalid address\n");
    }
    //connect
    Connect(sock, (const struct sockaddr *) &server_addr, sizeof(server_addr));
    //send & recv in while-loop
    while(1) {
	read_set = sock_set;
	Select(sock+1,&read_set,NULL,NULL,NULL);
	if(FD_ISSET(sock, &read_set)){ 			// socket lesbar? 
	    size = Recv(sock, (void *) buf, (size_t) sizeof(buf),0);
	    if (size == 0)
		break;
	    else
	    	Write(1, buf, size);				// ausgeben
	}

	if(FD_ISSET(0,&read_set)) {			// stdin lesbar?
	    // solange read 0 -> shutdown, sonst send 
	    size = Read(0, buf, sizeof(buf));
	    if (size == 0) {
		shutdown(sock, SHUT_RD);
		FD_CLR(0,&sock_set);
	    } else {
	        Send(sock, (void *) buf, size, 0);	// senden
	    }
	}

    }
    Close(sock);
    return 0;
}









