#define SYSTEMID_FLIGHT (uint8_t)0x00
#define SYSTEMID_ASPECT (uint8_t)0x40
#define SYSTEMID_CARDCAGE (uint8_t)0x80

#define TMTYPE_ACK (uint8_t)0x01
#define TMTYPE_CUSTOM (uint8_t)0x10

#define IP_LOOPBACK "127.0.0.1"
#define PORT_TM 60501

#include <unistd.h> // for usleep
#include <iostream>
#include <stdlib.h>

#include "UDPSender.hpp"
#include "Telemetry.hpp"
#include "types.hpp"

uint32_t count = 0;

int main()
{
    TelemetrySender telSender(IP_LOOPBACK, PORT_TM);

    while(1)    // run forever
    {
        usleep(500000);

        count++;
        TelemetryPacket tp(SYSTEMID_FLIGHT, TMTYPE_CUSTOM);

        tp.setCounter(count);

        Clock time = 0x123456789ABCDE;
        tp.setSystemTime(time);

        tp << (double)(count);
        if(count % 2) tp << (float)count;

        std::cout << tp << std::endl;
        std::cout << "Packet size: " << tp.getReadIndex()+tp.remainingBytes() << std::endl;

        telSender.send( &tp );
    }

    return 0;
}

