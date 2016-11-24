//
// Created by todsagt on 24.11.16.
//

#include "server.h"

void Perror( char *string, int e ) {
    perror( string );
    exit( e );
}

void printsyn() {
    printf( "SYNOPSIS\n" );
    printf( "\tserver [-b <behave>] [-p <port>]\n" );
    printf( "\n" );
    printf( "OPTIONS\n" );
    printf( "\t-b <behave>\n" );
    printf( "\t\tBehave as said in <behave>. Default is ECHO.\n" );
    printf( "\t-p <port>\n" );
    printf( "\t\tListen on port <port>. <port> should be an integer between 0 and 65535. Default port is 6666.\n" );
    printf( "\n" );
    printf( "BEHAVIOR\n" );
    printf( "\tCHARGEN | 0\n" );
    printf( "\t\tStart a server and behave as in rfc 864.\n" );
    printf( "\tDAYTIME | 1\n" );
    printf( "\t\tStart a server and behave as in rfc 867.\n" );
    printf( "\tDISCARD | 2\n" );
    printf( "\t\tStart a server and behave as in rfc 863.\n" );
    printf( "\tECHO | 3\n" );
    printf( "\t\tStart a server and behave as in rfc 862.\n" );
    exit( -1 );
}

int get_args( int argc, char* argv[], int *behave, long *port ) {
    //vars
    int i, setb, setp;
    char *strtolcheck;
    long portl;
    setb = 0;
    setp = 0;

    if( ( argc % 2 != 1 ) || ( argc > ( 2 * NOPTIONS + 1 ) ) ) {
        printsyn();
    }
    for( i = 1; i < argc; i += 2 ) {
        if( !strncmp( argv[i], "-b", 2 ) ) {
            if( ( setb == 1 ) || ( strlen( argv[i] ) != 2 ) ) {
                printsyn();
            }
            setb = 1;
            if( !strncasecmp( argv[i+1], "CHARGEN", 7 ) || !strncmp( argv[i+1], "0", 1 ) ) {
                *behave = CHARGEN;
            } else if( !strncasecmp( argv[i+1], "DAYTIME", 7 ) || !strncmp( argv[i+1], "1", 1 ) ) {
                *behave = DAYTIME;
            } else if( !strncasecmp( argv[i+1], "DISCARD", 7 ) || !strncmp( argv[i+1], "2", 1 ) ) {
                *behave = DISCARD;
            } else if( !strncasecmp( argv[i+1], "ECHO", 4 ) || !strncmp( argv[i+1], "3", 1 ) ) {
                *behave = ECHO;
            } else {
                printsyn();
            }
        } else if( !strncmp( argv[i], "-p", 2 ) ) {
            if( ( setp == 1 ) || ( strlen( argv[i] ) != 2 ) ) {
                printsyn();
            }
            setp = 1;
            portl = strtol( argv[i+1], &strtolcheck, 0 );
            if( *strtolcheck != '\0' || portl < 0 || portl > 65535 ) {
                printf( "Error getting port. Using %d as port.\n", PORT );
            } else {
                *port = portl;
            }
        } else {
            printsyn();
        }
    }
    return 0;
}
int setup( long port ) {
    //vars
    int ret, fd;
    struct sockaddr_in6 server;

    fd = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
    if( fd == -1 ) Perror( "socket:", -1 );
    ret = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof( int ) );
    if( ret == -1 ) Perror( "setsockopt:", -1 );
    ret = setsockopt( fd, IPPROTO_IPV6, IPV6_V6ONLY, &(int){ 0 }, sizeof( int ) );
    if( ret == -1 ) Perror( "setsockopt:", -1 );
    memset( &server, 0, sizeof( struct sockaddr_in ) );

#ifdef HAVE_SIN_LEN
    server.sin6_len = sizeof( struct sockaddr_in );
#endif

    server.sin6_family = AF_INET6;
    server.sin6_port = htons( port );
    server.sin6_addr = in6addr_any;
    ret = bind( fd, (const struct sockaddr *) &server, sizeof( server ) );
    if( ret == -1 ) Perror( "bind:", -1 );
    ret = listen( fd, 0 );
    if( ret == -1 ) Perror( "listen:", -1 );
    return fd;
}

char chars( char **msg, int len, char __char ) {
    // vars
    int i;
    char _char;

    // first char
    _char = __char;

    // generate "string"
    (*msg) = malloc( len * sizeof( char ) );
    for( i = 0; i < ( len - 1 ); i++ ) {
        (*msg)[i] = _char;
        _char = ( _char + 1 ) % 127; // ASCII
        if( _char == 0 ) _char += 33; // first printable char
    }
    (*msg)[len - 1] = '\0';
    return _char;
}