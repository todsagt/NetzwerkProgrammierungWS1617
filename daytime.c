//
// Created by todsagt on 24.11.16.
//

#include "server.h"
void *daytime( void *arg ) {
    // vars
    ssize_t err, len, tmp, send_flag;
    int client;
    time_t current_time;
    struct tm* local_time;
    char *stime, buf[MAX_MSG];

    client = *((int *) arg);
    free( arg );

    // sockopt SO_NOSIGPIPE for BSD
#if defined( __FreeBSD__ )
    err = setsockopt( client, SOL_SOCKET, SO_NOSIGPIPE, &(int){ 1 }, sizeof( int ) );
    if( err == -1 )
    {
        perror( "setsockopt" );
        close( client );
        return NULL;
    }
    send_flag = 0;
#else
    // MSG_NOSIGNAL for linux systems on send call
    send_flag = MSG_NOSIGNAL;
#endif

    // get time
    current_time = time( NULL );
    local_time = localtime( &current_time );
    stime = asctime( local_time );
    len = strlen( stime );
    err = 0;
    do {
        // send
        tmp = send( client, stime+err, len, send_flag );
        if( tmp == -1 ) {
#ifdef DEBUG
            perror( "send" );
#endif
            close( client );
            return NULL;
        }
        err += tmp;
    } while( err != len );

    // shut WR
    err = shutdown( client, SHUT_WR );
    if( err == -1 ) {
#ifdef DEBUG
        perror( "shutdown" );
#endif
        return NULL;
    }

    // receive
    do {
        len = recv( client, buf, MAX_MSG, 0 );
    } while( len > 0);

    // close doors
    close( client );
    return NULL;
}