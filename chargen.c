//
// Created by todsagt on 24.11.16.
//

#include "server.h"

void *chargen( void *arg ) {
    // vars
    int client, err, send_flag;
    ssize_t len, tmp, msg_len;
    char buf[MAX_MSG], *msg, _char;
    fd_set r_set, w_set;

    client = *((int *) arg);
    free( arg );

    // sockopt SO_NOSIGPIPE for BSD
#if defined( __FreeBSD__ )
    err = setsockopt( client, SOL_SOCKET, SO_NOSIGPIPE, &(int){ 1 }, sizeof( int ) );
    if( err == -1 ) {
        perror( "setsockopt:" );
        close( client );
        return NULL;
    }
    send_flag = 0;
#else
    // MSG_NOSIGNAL for linux systems on send call
    send_flag = MSG_NOSIGNAL;
#endif
    // random start char
    srand( time( NULL ) );
    _char = rand() % 93 + 33;
    do {
        // read fd set
        FD_ZERO( &r_set );
        FD_SET( client, &r_set );

        // write fd set
        FD_ZERO( &w_set );
        FD_SET( client, &w_set );

        // select
        err = select( client + 1, &r_set, &w_set, NULL, NULL );
        if( err == -1 ) {
#ifdef DEBUG
            perror( "select" );
#endif
            close( client );
            return NULL;
        }

        // receive
        if( FD_ISSET( client, &r_set ) ) {
            memset( buf, '\0', MAX_MSG );
            msg_len = recv( client, buf, MAX_MSG-1, 0 );
            if( msg_len <= 0 ) break;
        }

        // send
        if( FD_ISSET( client, &w_set ) ) {
            len = rand() % 1500 + 1;
            err = 0;
            _char = chars( &msg, len, _char );
            while( err != len && len != -1 ) {
                tmp = send( client, msg+err, len-err, send_flag );
                if( tmp == -1 ) {
#ifdef DEBUG
                    perror( "send" );
#endif
                    close( client );
                    free( msg ); // malloc in chars()
                    return NULL;
                }
                err += tmp;
            }
            free( msg ); // malloc in chars()
            if( msg_len == -1 ) break;
        }
    } while( 1 ); // hui
    close( client );
    return NULL;
}