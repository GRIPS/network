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
        uint16_t packet_length = udpreceiver.listen();
        if( packet_length != 0){
            uint8_t *packet = new uint8_t[packet_length];
            udpreceiver.get_packet(packet);

            std::cout << "Packet received, " << packet_length << " bytes\n";

            TelemetryPacket tp = TelemetryPacket(packet, packet_length);
            std::cout << "  Raw: " << tp << std::endl;

            if (tp.valid()){
                printf("  SystemID: 0x%02x\n", tp.getSystemID());
                printf("  TmType: 0x%02x\n", tp.getTmType());
                printf("  SystemTime: 0x%06llx\n", (uint64_t)tp.getSystemTime());
            } else {
                std::cout << "  Invalid packet!\n";
            }

            delete packet;
        }
    }

    return 0;
}
