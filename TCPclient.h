#ifndef TCPCLIENT_H_INCLUDED
#define TCPCLIENT_H_INCLUDED

#include <iostream> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sstream>
#include<cstdio>

#define RCVBUFSIZE 32   /* Size of receive buffer */

extern int createdSocket;
void DieWithError(char *errorMessage);  /* Error handling function */
void sendData(int , int , int , int , int , int );
int init_socket();


#endif // TCPCLIENT_H_INCLUDED
