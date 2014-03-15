/*

  CommandPacket and CommandPacketQueue
  derived from Packet and ByteStringQueue

  See Packet.hpp for details for how to work with ByteString-derived classes.
  The checksum and the payload length are calculated whenever the packet is
  "used": either with the method outputTo() or inserted into an ostream.
  You can use UDPSender-based and UDPReceiver-based classes to communicate with
  Packets.


  Create a new command packet with the header information:

  cp = CommandPacket(0x00, 0x10, 123);

  Load information using insertion operators:

  cp << (int16_t)57;
  cp << (uint8_t)123 << (double)1.56;

  Alternatively, create a command packet from a byte array:

  cp = CommandPacket(buffer, num_bytes);

  Example of how to extract data from a command packet:

  int16_t x;
  uint8_t y;
  double z;
  cp >> x >> y;
  cp >> z;

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
