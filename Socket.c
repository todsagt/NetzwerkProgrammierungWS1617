//
// Created by todsagt on 19.10.16.
//
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "Socket.h"
#include <sys/select.h>

int Socket(int socketd, int type, int protocol) {
    // Wraps the socket call in for error handling
    int n;
    if ((n=socket(socketd, type, protocol)) < 0) {
        perror("socket");
        exit(-1);
    }
    return(n);
}

int Bind(int socket, const struct sockaddr *server_addr, socklen_t addr_len) {
    // Wraps the bind call in for error handling
    int n;
    if ((n=bind(socket, server_addr, addr_len)) != 0) {
        perror("bind");
        exit(-1);
    }
    return(n);
}

int Connect(int socket, const struct sockaddr *server_addr, socklen_t addr_len) {
    // Wraps the connect call in for error handling
    int n;
    if ((n=connect(socket, server_addr, addr_len)) != 0) {
        perror("connect");
        exit(-1);
    }
    return(n);
}

int Select(int nfds,fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    // Wraps the select call in for error handling
    int n;
    if ((n=select(nfds, readfds, writefds, exceptfds, timeout)) == -1) {
        perror("select");
        exit(-1);
    }
    return(n);
}

ssize_t Recvfrom(int socket, void *buf, size_t buflen, int flags, struct sockaddr *from, socklen_t addr_len) {
    // Wraps the recvfrom call in for error handling
    ssize_t len;
    if ((len = recvfrom(socket, (void *) buf, buflen, flags, from, &addr_len)) == -1) {
        perror("recvfrom");
        exit(-1);
    }
    return(len);
}

ssize_t Recv(int socket, void *buf, size_t buflen, int flags) {
    // Wraps the recv call in for error handling
    ssize_t len;
    if ((len = recv(socket, (void *) buf, buflen, flags)) == -1) {
        perror("recv");
        exit(-1);
    }
    return(len);
}

ssize_t Read(int socket, void *buf, size_t read_len) {
    // Wraps the read call in for error handling
    ssize_t len;
    if ((len = read(socket, &buf, read_len)) == -1) {
        perror("read");
        exit(-1);
    }
    return(len);
}

int Accept(int socket, struct sockaddr* addr, socklen_t *len) {
    // Wraps the read call in for error handling
    int n;
    if((n = accept(socket, addr, len)) == -1) {
        perror("accept");
        exit(-1);
    }
    return(n);
}

int Listen(int socket, int backlog) {
    // Wraps the read call in for error handling
    if (listen(socket, backlog) == -1) {
        perror("listen");
        exit(-1);
    }
    return 0;
}

ssize_t Sendto(int socket, const void *buf, size_t buf_len, int flags, const struct sockaddr *client_addr, socklen_t addr_len) {
    // Wraps the sendto call in for error handling
    int len;
    if((len = sendto(socket, &buf, (size_t) buf_len, flags, client_addr, addr_len)) == -1) {
        perror("sendto");
        exit(-1);
    }
    return(len);
}

ssize_t Send(int socket, void *buf, size_t buf_len, int flags) {

    ssize_t len;
    if ((len = send(socket, &buf, buf_len, flags)) == -1) {
        perror("send");
        exit(-1);
    }
    return(len);
}

ssize_t Write(int sock, const void *buf, size_t buf_len) {

    ssize_t len;
    if ((len=write(sock, buf, buf_len)) == -1) {
	perror("write");
	exit(-1);
    }
    return(len);
}

int Close(int socket) {
    // Wraps the close call in for error handling
    if (close(socket) < 0) {
        perror("close");
        exit(-1);
    }
    return(0);
}
