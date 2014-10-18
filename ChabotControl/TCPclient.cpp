#include <TCPclient.h>

using namespace std;



int init_socket()
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in ServAddr; /* server address */
    unsigned short ServPort;     /* server port */
    char *servIP;                    /* Server IP address*/



    servIP = "10.42.0.56";
    ServPort = 777;


    /* Create a reliable, stream socket using TCP */

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");



    /* Construct the server address structure */

    memset(&ServAddr, 0, sizeof(ServAddr));     /* Zero out structure */
    ServAddr.sin_family      = AF_INET;             /* Internet address family */
    ServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    ServAddr.sin_port        = htons(ServPort);     /* Server port */



    /* Establish the connection to the server */

    if (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
        DieWithError("connect() failed");


    /*int elbow = 1;
    int leftStraight = 30;
    int leftTurn = 40;
    int rightStraight = 50;
    int rightTurn = 60;

    sentAngle(elbow, leftStraight, leftTurn, rightStraight, rightTurn, sock);*/
//	createPacket(100,24,-22,54,-10,sock);
    //close(sock);
    return sock;

}



void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void sendData(int packetType, int left,int right, int sock)
{
    unsigned int MsgStringLen;      /* Length of string*/
    char MsgString [20] ;

    ostringstream packet;
    packet<<packetType<<","<<left<<","<<right<<",";
    //cout<<"packet:"<<packet.str()<<endl;
    //cout<<"packetsize:"<<packet.str().size()<<endl;


    /*
    sprintf(MsgString,"%i",elbow/10);
    sprintf(MsgString+1,"%i",elbow%10);
    sprintf(MsgString+2,"-");
    sprintf(MsgString+3,"%i",leftStraight/10);
    sprintf(MsgString+4,"%i",leftStraight%10);
    sprintf(MsgString+5,"-");
    sprintf(MsgString+6,"%i",leftTurn/10);
    sprintf(MsgString+7,"%i",leftTurn%10);
    sprintf(MsgString+8,"-");
    sprintf(MsgString+9,"%i",rightStraight/10);
    sprintf(MsgString+10,"%i",rightStraight%10);
    sprintf(MsgString+11,"-");
    sprintf(MsgString+12,"%i",rightTurn/10);
    sprintf(MsgString+13,"%i",rightTurn%10);

    printf(" %s \n",MsgString);*/
    //MsgStringLen = strlen(MsgString);          /* Determine input length */
    const char* packetStr=packet.str().data();

    /* Send the string to the server */
    //sendto(sock, packetStr, packet.str().size(),0, (struct sockaddr *) &ServAddr, sizeof(ServAddr));
    if (send(sock, packetStr, packet.str().size(), 0) != packet.str().size()) {}
    //DieWithError("send() sent a different number of bytes than expected");

}
