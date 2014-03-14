/*

  TelemetryPacket and TelemetryPacketQueue
  derived from Packet and ByteStringQueue

  See Packet.hpp for details for how to work with ByteString-derived classes

  Example of how to create a new telemetry packet:

  tp = TelemetryPacket(0x00, 0x10, 1234, 13579);

  Example of how to add data to a telemetry packet's payload:

  tp << (uint16_t)57 << (double)1.56;

  Example of how to create a telemetry packet from a byte array:

  tp = TelemetryPacket(buffer, num_bytes);

  Example of how to extract data from such a telemetry packet:

  tp >> x >> y;

*/

#ifndef _TELEMETRY_HPP_
#define _TELEMETRY_HPP_

#include <list>
#include <iostream>

#include "Packet.hpp"
#include "types.hpp"

#define TELEMETRY_PACKET_MAX_SIZE 1024

class TelemetryPacket : public Packet {
protected:
    virtual void finish();
    virtual void writePayloadLength();
    virtual void writeChecksum();

public:
    //Use this constructor when assembling a telemetry packet for sending
    TelemetryPacket(uint8_t systemID, uint8_t tmType, uint16_t counter, Clock systemTime);

    //Use this constructor when handling a received telemetry packet
    TelemetryPacket(const uint8_t *ptr, uint16_t num);

    //Use this constructor when needing to have an empty telemetry packet
    //Pass in NULL
    //This packet is non-functional!  Be sure not to use without reassignment!
    TelemetryPacket(const void *ptr);

    //Checks for the GRIPS sync word and a valid checksum
    virtual bool valid();

    uint8_t getSystemID();
    uint8_t getTmType();
    
    uint16_t getSync();

    uint16_t getCounter();
    Clock getSystemTime();
};

class TelemetryPacketQueue : public ByteStringQueue {
private:
    uint8_t i_systemID;
    uint8_t i_tmType;
    bool filter_systemID;
    bool filter_tmType;

public:
    TelemetryPacketQueue();

    //Adds the telemetry packets from a static file
    void add_file(const char *file);

    void filterSystemID(uint8_t systemID);
    void filterTmType(uint8_t tmType);
    void resetFilters();
};

#endif
