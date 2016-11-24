//
// Created by todsagt on 24.11.16.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main( int argc, char* argv[] ) {
    //vars
    int e;
    struct addrinfo *ip, *step;
    socklen_t sa_len;
    char ipaddr[NI_MAXHOST];

    if( argc != 2 ) {
        printf( "Geben sie einen Hostnamen an.\n" );
        return -1;
    }

    e = getaddrinfo( argv[1], NULL, NULL, &ip );

    if( e != 0 ) {
        perror( "getaddrinfo" );
        return -1;
    }

    for( step = ip; step != NULL; step = step->ai_next ) {

#if defined( __FreeBSD__ )
        sa_len = step->ai_addr->sa_len;
#else
        sa_len = sizeof( struct sockaddr_in6 );
#endif

        e = getnameinfo( step->ai_addr, sa_len, ipaddr, sizeof( ipaddr ), NULL, 0, NI_NUMERICHOST );

        if( e != 0 ) {
            perror( "getnameinfo" );
            return -1;
        }

        printf( "ai_family -> %i\n", step->ai_family );
        printf( "ai_socktype -> %i\n", step->ai_socktype );
        printf( "ai_protocol -> %i\n", step->ai_protocol );
        printf( "IP: %s\n\n", ipaddr );
    }
    freeaddrinfo( ip );
    return 0;
}

