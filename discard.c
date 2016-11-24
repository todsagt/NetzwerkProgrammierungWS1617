//
// Created by todsagt on 24.11.16.
//

#include "server.h"

void *discard( void *arg ) {
    // vars
    int client, len;
    char buf[MAX_MSG];

    client = *((int *) arg);
    free( arg );

    do {
        len = recv( client, buf, MAX_MSG, 0 ); // drop it
    } while( len > 0 );

    close( client );
    return NULL;
}