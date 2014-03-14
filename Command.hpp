/*

  CommandPacket and CommandPacketQueue
  derived from Packet and ByteStringQueue

*/

#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

#include <list>
#include <iostream>

#include "Packet.hpp"

#define COMMAND_PACKET_MAX_SIZE 262

class CommandPacket : public Packet {
private:
    virtual void finish();
    void writePayloadLength();
    void writeChecksum();

public:
    //Use this constuctor when assembling a command packet for sending
    CommandPacket(uint8_t systemID, uint8_t cmdType, uint8_t counter);

    //Use this constructor when handling a received command packet
    CommandPacket(const uint8_t *ptr, uint16_t num);

    //Use this constructor when needing to have an empty comamnd packet
    //Pass in NULL
    //This packet is non-functional!  Be sure not to use without reassignment!
    CommandPacket(const void *ptr);

    //Checks for the GRIPS sync word and a valid checksum
    virtual bool valid();

    uint8_t getSystemID();
    uint8_t getCmdType();

    uint8_t getCounter();
};

class CommandPacketQueue : public ByteStringQueue {

public:
    CommandPacketQueue() {};
};

#endif
