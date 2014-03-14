#include <iostream>

#include "Command.hpp"

#define INDEX_CHECKSUM 2
#define INDEX_SYSTEMID 4
#define INDEX_CMDTYPE 5
#define INDEX_COUNTER 6
#define INDEX_PAYLOAD_LENGTH 7
#define INDEX_PAYLOAD 8

using std::ostream;

class CommandPacketInvalidException : public std::exception
{
    virtual const char* what() const throw()
        {
            return "CommandPacket is not valid (syncword or checksum)";
        }
} cpInvalidException;

class CommandPacketSizeException : public std::exception
{
    virtual const char* what() const throw()
        {
            return "CommandPacket payload is too large";
        }
} cpSizeException;

class CommandPacketEndException : public std::exception
{
    virtual const char* what() const throw()
        {
            return "CommandPacket end reached";
        }
} cpEndException;

CommandPacket::CommandPacket(uint8_t systemID, uint8_t cmdType, uint8_t counter)
{
    //Zeros are checksum and payload length
    *this << (uint16_t)0 << systemID << cmdType << counter << (uint8_t)0;
    setReadIndex(INDEX_PAYLOAD);
}

CommandPacket::CommandPacket(const uint8_t *ptr, uint16_t num)
    : Packet(ptr, num)
{
    setReadIndex(INDEX_PAYLOAD);
}

CommandPacket::CommandPacket(const void *ptr)
{
    //Assumes that NULL was passed in
}

void CommandPacket::finish()
{
    writePayloadLength();
    writeChecksum();
}

void CommandPacket::writePayloadLength()
{
    if(getLength() > COMMAND_PACKET_MAX_SIZE) throw cpSizeException;
    //should check if payload length is even
    //should also check if payload length is >= 2 bytes
    replace(INDEX_PAYLOAD_LENGTH, (uint8_t)(getLength()-INDEX_PAYLOAD));
}

void CommandPacket::writeChecksum()
{
    replace(INDEX_CHECKSUM, (uint16_t)0);
    replace(INDEX_CHECKSUM, (uint16_t)checksum());
}

bool CommandPacket::valid()
{
    return Packet::valid();
}

uint8_t CommandPacket::getSystemID()
{
    uint8_t value;
    this->readAtTo( INDEX_SYSTEMID, value);
    return value;
}

uint8_t CommandPacket::getCounter()
{
    uint8_t value;
    this->readAtTo(INDEX_COUNTER, value);
    return value;
}
