#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <string>
#include "chabot.h"


#define MAXPENDING 1   /* Maximum outstanding connection requests */
#define RCVBUFSIZE 32   /* Size of receive buffer */

using namespace std;
void * recvToAngle(char *BufferRec);

int piece_int[5];
//int init_server();
int main()
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in ServAddr; 	 /* Local address */
    struct sockaddr_in ClntAddr; 	 /* Client address */
    unsigned short ServPort;     	 /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */
	char Buffer[RCVBUFSIZE];        /* Buffer for string */
	int j;
	int *angles;
	init_chabot();
    ServPort = 777;  			     /* First arg:  local port */

    /* Create socket for incoming connections */

    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
       { perror("socket failed"); exit(1);}



    /* Construct local address structure */

    memset(&ServAddr, 0, sizeof(ServAddr));   /* Zero out structure */
    ServAddr.sin_family = AF_INET;                /* Internet address family */
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    ServAddr.sin_port = htons(ServPort);      /* Local port */



    /* Bind to the local address */

    if (bind(servSock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
        { perror("bind failed"); exit(1);}



    /* Mark the socket so it will listen for incoming connections */

    if (listen(servSock, MAXPENDING) < 0)
        { perror("listen failed"); exit(1);}


        /* Set the size of the in-out parameter */
        clntLen = sizeof(ClntAddr);

         /* Wait for a client to connect */
      				  if ((clntSock = accept(servSock, (struct sockaddr *) &ClntAddr, &clntLen)) < 0)
			          { perror("accept failed"); exit(1);}

			        /* clntSock is connected to a client! */

			        printf("Handling client %s\n", inet_ntoa(ClntAddr.sin_addr));

		int recvMsgSize;                    /* Size of received message */

		/* Receive message from client */
		int cnt = 0;
		do
		{
				
				//cout<<"recieving"<<endl;
				if ((recvMsgSize = recv(clntSock, Buffer, RCVBUFSIZE, 0)) < 0)
				{ 
					perror("recv failed"); //exit(1);
					continue;
				}

			
				recvToAngle(Buffer);
			cnt++;
				if (cnt < 10) {
					continue;
				}
				if (cnt>16) {
					cnt=0;
				}
				switch (piece_int[0]) {
					case 1024:
						checkIncr(3,2,piece_int[1]);
						checkIncr(2,2,piece_int[2]);
						break;
					case 512:
						checkIncr(3,3,piece_int[1]);
						checkIncr(2,3,piece_int[2]);							
							break;
					case 256:
						checkIncr(3,1,piece_int[1]);
						checkIncr(2,1,piece_int[2]);
						break;

					default:
						break;
				}
				
				


		}while(true);



		close(clntSock);    /* Close client socket */
		destroy_runner();
		return(0);
    }


    void * recvToAngle(char *BufferRec)
    {
	
		  int i = 0;
		  char * piece;
		  
		  piece = strtok (BufferRec,",");
		cout<<"recieved Value:"<<piece<<std::endl;			
		if(piece!=NULL){
		  piece_int[i] = atoi(piece);
		}
		  
		 // printf("elbow or shoulder = %i \n",piece_int[i]);


		  while (i<3)
		  {
			//;printf ("%s   \n",test);
			i++;
			piece = strtok (NULL, ",");
			if(piece!=NULL){  
		 	 piece_int[i] = atoi(piece);
			}

			//printf("angle %i = %i \n",i,piece_int[i]);
		  }

		


		



	}

