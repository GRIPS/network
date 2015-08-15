#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <fcntl.h>      /* for fcntl() */
#include "lib_crc/lib_crc.h"

#include "UDPReceiver.hpp"

UDPReceiver::UDPReceiver( unsigned short port ){
    listeningPort = port;
    sock = -1;
}

UDPReceiver::~UDPReceiver() {
    close_connection();
}

int UDPReceiver::listen( void ){
    /* Receive a message from a client or times out */
    recvMsgSize = recvfrom(sock, payload, sizeof(payload), 0,
                           (struct sockaddr *) &senderAddr, &cliAddrLen);
    return recvMsgSize;
}

void UDPReceiver::init_connection( void ){
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        printf("UDPReceiver: socket() failed\n");

    /* Construct local address structure */
    memset(&myAddr, 0, sizeof(myAddr));   /* Zero out structure */
    myAddr.sin_family = AF_INET;                /* Internet address family */
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    myAddr.sin_port = htons(listeningPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &myAddr, sizeof(myAddr)) < 0)
        printf("UDPReceiver: bind() failed\n");

    /* Set the size of the in-out parameter */
    cliAddrLen = sizeof(senderAddr);

    /* Set a one-second timeout */
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

void UDPReceiver::get_packet( uint8_t *packet ){
    memcpy( packet, payload, recvMsgSize);
}

void UDPReceiver::close_connection( void ){
    if (sock >= 0) {
        close( sock );
        sock = -1;
    }
}

CommandReceiver::CommandReceiver( unsigned short port )
    : UDPReceiver(port)
{
}

TelemetryReceiver::TelemetryReceiver( unsigned short port )
    : UDPReceiver(port)
{
}




