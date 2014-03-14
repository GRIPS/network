/*

  Command, CommandPacket, CommandQueue, and CommandPacketQueue
  derived from ByteString, Packet, ByteStringQueue

  To create a command:
  Command cm1(0x1100);
  Command cm2(0x1102);
  ByteString solution;
  solution << (double)1 << (double)10 << (double)100 << (double)1000;
  solution << (uint32_t)5 << (uint16_t)6;
  cm2 << solution;
  Command cm3(0x10ff, 0x1234);

  To create a command packet:
  CommandPacket cp1(0x01, 100); //target ID and sequence number
  cp << cm1 << cm2 << cm3;

  To get a uint8_t array from the command packet:
  uint16_t length = cp1.getLength();
  uint8_t *array = new uint8_t[length];
  cp1 >> array; //or cp1.outputTo(array);

  To get a uint8_t array into a command packet:
  CommandPacket cp2(array, length);

  To get a queue of commands from a command packet:
  CommandQueue cq;
  if(cp2.valid()) cq.add_packet(cp2);

  To extract the next command from a queue of commands:
  Command c;
  if(!cq.empty()) cq >> c;

  To interpret the command:
  c.get_heroes_command();
  c.get_sas_command(); //returns 0 if the HEROES command is not 0x10ff

  Read the first uint16_t from an individual command's payload (after the keys):
  uint16_t x;
  c >> x; //or c.readNextTo(x);


  Notes:
  - Will throw an exception when a Command with an incorrect number of associated
  bytes (its payload) is attempted to be added to a CommandPacket
  - Currently, all SAS commands have a zero-length payload
  - A proper checksum and payload length are written to the HEROES header of a
  packet when and only when either outputted to an array or to an ostream

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
    CommandPacket(uint8_t systemID, uint8_t cmdType);

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
