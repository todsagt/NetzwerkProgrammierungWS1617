//
// Created by todsagt on 19.10.16.
//

#include "Socket.h"

Socket(int fd, int type, int protocol)
{
    int n;
    if ((n=socket(fd, type, protocol)) < 0) {
        perror(„socket“);
        exit(-1);
    }
    return(n);
}

