#include <iostream>
#include <stdio.h>

#include "Command.hpp"
#include "UDPReceiver.hpp"

#define PORT_CMD 50501

int main(int argc, char *argv[])
{
    UDPReceiver udpreceiver = UDPReceiver(PORT_CMD);
    udpreceiver.init_connection();

    while(1){
        int packet_length = udpreceiver.listen();
        if( packet_length > 0){
            uint8_t *packet = new uint8_t[packet_length];
            udpreceiver.get_packet(packet);

            std::cout << "Packet received, " << packet_length << " bytes\n";

            CommandPacket cp = CommandPacket(packet, packet_length);

            if (cp.valid()){
                std::cout << "  Raw: " << cp << std::endl;
                printf("  SystemID: 0x%02x\n", cp.getSystemID());
                printf("  CmdType: 0x%02x\n", cp.getCmdType());
            } else {
                std::cout << "  Invalid packet!\n";
            }

            delete packet;
        }
    }

    return 0;
}
