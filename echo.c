//
// Created by todsagt on 24.11.16.
//

#include "server.h"

void *echo( void *arg ) {
    // vars
    int client, send_flag;
    ssize_t len, err, tmp;
    char buf[MAX_MSG];

    client = *((int *) arg);
    free( arg );

    // sockopt SO_NOSIGPIPE for BSD
#if defined( __FreeBSD__ )
    err = setsockopt( client, SOL_SOCKET, SO_NOSIGPIPE, &(int){ 1 }, sizeof( int ) );
    if( err == -1 ) {
        perror( "setsockopt" );
        close( client );
        return NULL;
    }
    send_flag = 0;
#else
    // MSG_NOSIGNAL for linux systems on send call
    send_flag = MSG_NOSIGNAL;
#endif
    do {
        memset( buf, 0, MAX_MSG );
        err = 0;

        // receive
        len = recv( client, buf, MAX_MSG-1, 0 );

        // send
        while( err != len && len != -1 ) {
            tmp = send( client, buf+err, len-err, send_flag ); // and back
            if( tmp == -1 ) {
#ifdef DEBUG
                perror( "send" );
#endif
                close( client );
                return NULL;
            }
            err += tmp;
        }
    } while( len > 0 );

    close( client );
    return NULL;
}