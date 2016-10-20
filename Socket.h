//
// Created by todsagt on 19.10.16.
//
// Wrapperfunctions for "automatet" errorhandling and easier Servercode
//

#ifndef NETZWERKPROGRAMMIERUNGWS1617_SOCKET_H
#define NETZWERKPROGRAMMIERUNGWS1617_SOCKET_H
#endif //NETZWERKPROGRAMMIERUNGWS1617_SOCKET_H

int Socket(int socket, int type, int protocol);
int Bind(int socket, const struct sockaddr *server_addr, socklen_t addrlen);
int Connect(int socket, const struct sockaddr *addr, socklen_t len);
ssize_t Recvfrom(int socket, void *buf, size_t buflen, int flags, struct sockaddr *from, socklen_t addr_len);
ssize_t Recv(int fd, void *buf, size_t buflen, int flags);
ssize_t Sendto(int socket, const void *buf, size_t buf_len, int flags, const struct sockaddr *to, socklen_t addr_len);
ssize_t Send(int fd, void *buf, size_t buflen, int flags);
int Close(int socket);