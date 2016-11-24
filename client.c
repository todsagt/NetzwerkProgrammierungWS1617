//
// Created by todsagt on 24.11.16.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_MSG (1<<16)

void Perror( char *string, int e ) {
    perror( string );
    exit( e );
}

int send_msg( int sock ) {
    // vars
    char buf[MAX_MSG];
    ssize_t len, err = 0, tmp;

    // read from stdin
    memset( buf, 0, MAX_MSG );
    len = read( 0, buf, MAX_MSG-1 );
    if( len <= 0 ) {
        // Close connection clientside
        err = shutdown( sock, SHUT_WR );
        if( err == -1 ) Perror( "shutdown", -1 );
        return 1;
    } else {
        // Send everything
        while( err != len ) {
            tmp = send( sock, buf+err, len-err, 0 );
            if( tmp == -1 ) Perror( "send", -1 );
            err += tmp;
        }
    }
    return 0;
}

void run( int sock ) {
    // vars
    int r = 1, err;
    char buf[MAX_MSG];
    ssize_t len;
    fd_set rsock_set;

    // run
    while( 1 ) {
        // sock_set
        FD_ZERO( &rsock_set );
        if( r ) FD_SET( 0, &rsock_set );
        FD_SET( sock, &rsock_set );

        // wait for readable filedescriptor
        err = select( sock + 1, &rsock_set, NULL, NULL, NULL );
        if( err == -1 ) Perror( "select", -1 );

        // send message
        if( FD_ISSET( 0, &rsock_set ) ) {
            err = send_msg( sock );
            if( err == 1 ) {
                FD_CLR( 0, &rsock_set );
                r = 0;
            }
        }

        // receive message
        if( FD_ISSET( sock, &rsock_set ) ) {
            memset( buf, 0, MAX_MSG );
            len = recv( sock, buf, MAX_MSG-1, 0 );

            if( len <= 0 ) break;
            printf( "%s", buf );
        }
    }
}

int try_host( struct addrinfo host ) {
    // vars
    int sock, ret;

    // socket
    sock = socket( host.ai_family, host.ai_socktype, host.ai_protocol );

    // connect
    ret = connect( sock, host.ai_addr, host.ai_addrlen );
    if( ret != -1 ) ret = sock;
    else close( sock );

    return ret;
}

int main(int argc, char* argv[] ) {
    // vars
    int error, sock;
    struct addrinfo *hosts, *step, hints;

    // validate args
    if( argc != 3 ) {
        printf( "Bitte geben sie Adresse und Port an.\n" );
        return -1;
    }

    // hints and get address info
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    error = getaddrinfo( argv[1], argv[2], &hints, &hosts );
    if( error != 0 ) {
        gai_strerror( error );
        return -1;
    }

    // connect
    for( step = hosts; step != NULL; step = step->ai_next ) {
        sock = try_host( *step );
        if( sock != -1 ) {
            printf( "Verbindung hergestellt.\n" );
            run( sock );
            close( sock );
            freeaddrinfo( hosts );
            printf( "Verbindung geschlossen.\n" );
            return 0;
        }
    }
    printf( "Verbindung konnte nicht hergestellt werden.\n" );
    freeaddrinfo( hosts );
    return -1;
}