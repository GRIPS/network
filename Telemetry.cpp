#include <sys/time.h>
#include <time.h>

#include <fstream>
#include <iostream>

#include "Telemetry.hpp"

#define INDEX_CHECKSUM 2
#define INDEX_SYSTEM_ID 4
#define INDEX_TELEMETRY_TYPE 5
#define INDEX_PAYLOAD_LENGTH 6
#define INDEX_COUNTER 8
#define INDEX_SYSTEMTIME 10
#define INDEX_PAYLOAD 16

using std::ostream;

class TelemetryPacketSizeException : public std::exception
{
    virtual const char* what() const throw()
        {
            return "TelemetryPacket payload is too large";
        }
} tpSizeException;

class TelemetryPacketSASException : public std::exception
{
    virtual const char* what() const throw()
        {
            return "TelemetryPacket error manipulating SAS ID";
        }
} tpSASException;

TelemetryPacket::TelemetryPacket(uint8_t systemID, uint8_t tmType, uint16_t counter, Clock systemTime)
{
    //Zeros are checksum and payload length
    *this << (uint16_t)0 << systemID << tmType << (uint16_t)0 << counter;
    *this << (uint32_t)(systemTime & 0xFFFFFFFF);
    *this << (uint16_t)((systemTime >> 32) & 0xFFFF);
    setReadIndex(INDEX_PAYLOAD);
}

TelemetryPacket::TelemetryPacket(const uint8_t *ptr, uint16_t num)
    : Packet(ptr, num)
{
    setReadIndex(INDEX_PAYLOAD);
}

TelemetryPacket::TelemetryPacket(const void *ptr)
{
    //Assumes that NULL was passed in
}

void TelemetryPacket::finish()
{
    writePayloadLength();
    writeChecksum();
}

void TelemetryPacket::writePayloadLength()
{
    if(getLength() > TELEMETRY_PACKET_MAX_SIZE) throw tpSizeException;
    replace(INDEX_PAYLOAD_LENGTH, (uint16_t)(getLength()-INDEX_PAYLOAD));
}

void TelemetryPacket::writeChecksum()
{
    replace(INDEX_CHECKSUM, (uint16_t)0);
    replace(INDEX_CHECKSUM, (uint16_t)checksum());
}

bool TelemetryPacket::valid()
{
    return Packet::valid();
}

uint8_t TelemetryPacket::getTmType()
{
    uint8_t value;
    this->readAtTo(INDEX_TELEMETRY_TYPE, value);
    return value;
}

uint8_t TelemetryPacket::getSystemID()
{
    uint8_t value;
    this->readAtTo(INDEX_SYSTEM_ID, value);
    return value;
}

uint16_t TelemetryPacket::getSync()
{
    uint16_t value;
    this->readAtTo(0, value);
    return value;
}

uint16_t TelemetryPacket::getCounter()
{
    uint16_t value;
    this->readAtTo(INDEX_COUNTER, value);
    return value;
}

Clock TelemetryPacket::getSystemTime()
{
    uint32_t value1;
    uint16_t value2;
    Clock value;
    this->readAtTo(INDEX_SYSTEMTIME, value1);
    this->readAtTo(INDEX_SYSTEMTIME+4, value2);
    value = ((Clock)value2 << 32) + value1;
    return value;
}

TelemetryPacketQueue::TelemetryPacketQueue() : filter_systemID(false), filter_tmType(false)
{
}

void TelemetryPacketQueue::filterTmType(uint8_t tmType)
{
    filter_tmType = true;
    i_tmType = tmType;
}

void TelemetryPacketQueue::filterSystemID(uint8_t systemID)
{
    filter_systemID = true;
    i_systemID = systemID;
}

void TelemetryPacketQueue::resetFilters()
{
    filter_tmType = false;
    filter_systemID = false;
}

void TelemetryPacketQueue::add_file(const char* file)
{
    uint32_t ct_sync = 0, ct_length = 0, ct_valid = 0;
    uint32_t ct_tmType = 0, ct_systemID = 0;
    std::streampos cur;

    bool pass_systemID, pass_tmType;

    uint8_t buffer[TELEMETRY_PACKET_MAX_SIZE];
    buffer[0] = 0x9a;

    uint16_t length;

    TelemetryPacket tp(NULL);

    std::ifstream ifs(file);

    while (ifs.good()) {

        if(ifs.get() == 0x9a) {
            if(ifs.peek() == 0xc3) {
                ct_sync++; // sync word found

                cur = ifs.tellg(); // points one byte into sync word
                ifs.seekg(3, std::ios::cur);
                ifs.read((char *)&length, 2);

                if(length > TELEMETRY_PACKET_MAX_SIZE-16) continue; //invalid payload size
                ct_length++;

                ifs.seekg(cur);

                ifs.read((char *)buffer+1, length+15);

                tp = TelemetryPacket(buffer, length+16);

                if(tp.valid()) {
                    ct_valid++;
                    pass_systemID = !(filter_systemID && !(tp.getSystemID() == i_systemID));
                    pass_tmType = !(filter_tmType && !(tp.getTmType() == i_tmType));
                    if(pass_systemID) ct_systemID++;
                    if(pass_tmType) ct_tmType++;
                    if(pass_systemID && pass_tmType) *this << tp;
                }

                ifs.seekg(cur);
            }
        }

    }

    std::cout << ct_sync << " sync words found, ";
    std::cout << ct_valid << " packets with valid checksums\n";

    if(filter_systemID) {
        std::cout << ct_systemID << " packets with with the filtered source ID\n";
    }
    if(filter_tmType) {
        std::cout << ct_tmType << " packets with with the filtered type ID\n";
    }

}
