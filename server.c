//
// Created by todsagt on 24.11.16.
//

#include "server.h"

int main( int argc, char* argv[] ) {
    //vars
    int behave, sock, ret, *client;
    long port;
    pthread_t thread;
    struct sockaddr_in6 ipaddr;
    socklen_t len;
    void* ( *behaviour[4] ) ( void *arg );

    behaviour[CHARGEN] = chargen;
    behaviour[DAYTIME] = daytime;
    behaviour[DISCARD] = discard;
    behaviour[ECHO] = echo;
    behave = ECHO;
    port = PORT;
    ret = get_args( argc, argv, &behave, &port );
    if( ret == -1 ) return -1;
    sock = setup( port );
    while( 1 ) {
        memset( &ipaddr, 0, sizeof( struct sockaddr_in ) );
        len = (socklen_t) sizeof( ipaddr );
        client = malloc( sizeof( int ) );
        *client = accept( sock, (struct sockaddr *) &ipaddr, &len );
        if( *client == -1 ) {
#ifdef DEBUG
            perror( "accept" );
#endif
            continue;
        }
        pthread_create( &thread, NULL, behaviour[behave], client );
        pthread_detach( thread );
    }
    close( sock );
    return 0;
}
