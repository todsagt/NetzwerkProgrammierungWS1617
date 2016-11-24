//
// Created by todsagt on 24.11.16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pthread.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define CHARGEN 0
#define DAYTIME 1
#define DISCARD 2
#define ECHO 3
#define PORT 6666
#define NOPTIONS 3
#define MAX_MSG (1<<16)

void Perror( char *string, int e );
void printsyn();
int get_args( int argc, char* argv[], int *behave, long *port );
int setup( long port );
char chars( char **msg, int len, char __char );
void *chargen( void *arg );
void *daytime( void *arg );
void *discard( void *arg );
void *echo( void *arg );