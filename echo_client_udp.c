/*-
 * Copyright (c) 2013 Michael Tuexen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "Socket.h"

#define BUFFER_SIZE  (1<<16)
#define MESSAGE_SIZE (9216)
#define MESSAGE_SIZE_DEBUG (8)

int main(int argc, char **argv) {
    int connected = 1;
    if (connected == 0) {
        int socket;
        struct sockaddr_in server_addr, client_addr;
        ssize_t len;
        socklen_t addr_len;
        char buf[BUFFER_SIZE];

        socket = Socket(AF_INET, SOCK_DGRAM, 0);
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
        server_addr.sin_len = sizeof(struct sockaddr_in);
#endif
        server_addr.sin_port = htons(2500); // 2500 for executing without root, 7 otherwise
        if ((server_addr.sin_addr.s_addr = (in_addr_t) inet_addr(argv[1])) == INADDR_NONE) {
            fprintf(stderr, "Invalid address\n");
        }
        memset((void *) buf, 'A', sizeof(buf));
        len = Sendto(socket, (const void *) buf, (size_t) MESSAGE_SIZE, 0, (const struct sockaddr *) &server_addr,
               sizeof(server_addr));
        printf("Send %zd bytes to %s\n", len, inet_ntoa(server_addr.sin_addr));
        addr_len = (socklen_t) sizeof(client_addr);
        memset((void *) buf, 0, sizeof(buf));
        len = Recvfrom(socket, (void *) buf, sizeof(buf), 0, (struct sockaddr *) &client_addr, addr_len);
        printf("Received %zd bytes from %s\n", len, inet_ntoa(server_addr.sin_addr));
        Close(socket);
        return (0);


    } else if (connected == 1) { //connected UDP Socket
        // vars
        int socket;
        struct sockaddr_in server_addr;
        char buf[BUFFER_SIZE];
        ssize_t len;

        // socket call
        socket = Socket(AF_INET, SOCK_DGRAM, 0);
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
        server_addr.sin_len = sizeof(struct sockaddr_in);
#endif
        server_addr.sin_port = htons(2500);
        if ((server_addr.sin_addr.s_addr = (in_addr_t) inet_addr(argv[1])) == INADDR_NONE) {
            fprintf(stderr, "Invalid address\n");
        }

        // connect call
        Connect(socket, (const struct sockaddr *) &server_addr, sizeof(server_addr));
        // send call
        memset((void *) buf, 'A', sizeof(buf));
        len = Send(socket, (void *) buf, (size_t) MESSAGE_SIZE, 0);
        printf("Send %zd bytes to %s\n", len, inet_ntoa(server_addr.sin_addr));
        // recv call
        len = Recv(socket, (void *) buf, (size_t) sizeof(buf),0);
        printf("Received %zd bytes from %s\n", len, inet_ntoa(server_addr.sin_addr));
        // close call
        Close(socket);
        return (0);
    }
}
