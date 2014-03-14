/*

  TelemetryPacket and TelemetryPacketQueue
  derived from Packet and ByteStringQueue

//Telemetry packet from SAS containing an array
uint8_t image[5] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
TelemetryPacket tp2(0x70, 0x30);
tp2 << (uint32_t)0xEFBEADDE;
tp2.append_bytes(image, 5);
std::cout << tp2 << std::endl;

//Parsing telemetry packets from a static file
TelemetryPacketQueue tpq;
tpq.filterSourceID(0x30);
tpq.add_file("sample.dat");
TelemetryPacket tp(NULL);
if(!tpq.empty()) tpq >> tp;

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
    TelemetryPacket(uint8_t systemID, uint8_t tmType);

    //Use this constructor when handling a received telemetry packet
    TelemetryPacket(const uint8_t *ptr, uint16_t num);

    //Use this constructor when needing to have an empty telemetry packet
    //Pass in NULL
    //This packet is non-functional!  Be sure not to use without reassignment!
    TelemetryPacket(const void *ptr);

    //Checks for the HEROES sync word and a valid checksum
    virtual bool valid();

    uint8_t getSystemID();
    uint8_t getTmType();
    
    uint16_t getSync();

    uint16_t getCounter();
    void setCounter(uint16_t counter);

    Clock getSystemTime();
    void setSystemTime(Clock systemTime);
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
