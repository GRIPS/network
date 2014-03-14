#define SYSTEMID_FLIGHT (uint8_t)0x00
#define SYSTEMID_ASPECT (uint8_t)0x40
#define SYSTEMID_CARDCAGE (uint8_t)0x80

#define CMDTYPE_CUSTOM (uint8_t)0x10

#define IP_LOOPBACK "127.0.0.1"
#define PORT_CMD 50501

#include <unistd.h> // for usleep
#include <iostream>
#include <stdlib.h>

#include "UDPSender.hpp"
#include "Command.hpp"

int main()
{
    uint32_t count = 0;
    Clock clock = 0x123456789ABCDE;

    CommandSender cmdSender(IP_LOOPBACK, PORT_CMD);

    while(1)    // run forever
    {
        usleep(500000);

        count++;
        clock += 5000000;

        CommandPacket cp(SYSTEMID_FLIGHT, CMDTYPE_CUSTOM, count);

        cp << (double)(count);
        if(count % 2) cp << (float)count;

        std::cout << cp << std::endl;
        std::cout << "Packet size: " << cp.getReadIndex()+cp.remainingBytes() << std::endl;

        cmdSender.send( &cp );
    }

    return 0;
}

