#include <iostream>
#include <stdio.h>

#include "Telemetry.hpp"
#include "UDPReceiver.hpp"

#define PORT_TM 60501

int main(int argc, char *argv[])
{
    UDPReceiver udpreceiver = UDPReceiver(PORT_TM);
    udpreceiver.init_connection();

    while(1){
        int packet_length = udpreceiver.listen();
        if( packet_length > 0){
            uint8_t *packet = new uint8_t[packet_length];
            udpreceiver.get_packet(packet);

            std::cout << "Packet received, " << packet_length << " bytes\n";

            uint16_t reported_length = *((uint16_t *)(packet + 6));

            TelemetryPacket tp = TelemetryPacket(packet, packet_length);

            tp.readAtTo(6, reported_length);

            if (tp.valid()){
                std::cout << "  Raw: " << tp << std::endl;
                printf("  SystemID: 0x%02x", tp.getSystemID());
                printf("  TmType: 0x%02x", tp.getTmType());
                printf("  Length: %4d", tp.getLength());
                printf("  SystemTime: 0x%012lx\n", (uint64_t)tp.getSystemTime());

                if (reported_length+16 != packet_length) return -1;
            } else {
                std::cout << "  Invalid packet!\n";
            }

            delete packet;
        }
    }

    return 0;
}
