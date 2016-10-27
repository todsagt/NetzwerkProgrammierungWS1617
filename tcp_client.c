//
// Created by todsagt on 25.10.16.
//

#include <sys/types.h>
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
    //sock
    sock = Socket(AF_INET,SOCK_STREAM,0);
    signal(SIGINT, onClose);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
    server_addr.sin_len = sizeof(struct sockaddr_in);
#endif
    server_addr.sin_port = htons(13);
    if ((server_addr.sin_addr.s_addr = (in_addr_t) inet_addr(argv[1])) == INADDR_NONE) {
        fprintf(stderr, "Invalid address\n");
    }
    //connect
    Connect(sock, (const struct sockaddr *) &server_addr, sizeof(server_addr));
    //send & recv in while-loop
    while(1) {
        Send(sock, (void *) buf, (size_t) getline(&buf, &size, stdin)+1, 0); //+1 für \00
        printf("%.*s", (int) Recv(sock, (void *) buf, (size_t) sizeof(buf),0), buf);
    }
}
