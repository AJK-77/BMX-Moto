#include "Common/Protocol/Packet.h"
#include "Common/Constants.h"

Packet::Packet()
{
}

void Packet::clear()
{
    length = 0;
}

bool Packet::addByte(uint8_t value)
{
    if (length >= MAX_PACKET_SIZE)
    {
        return false;
    }

    data[length++] = value;

    return true;
}

bool Packet::addUInt16(uint16_t value)
{
    return addByte(value & 0xFF) &&
           addByte((value >> 8) & 0xFF);
}

bool Packet::addHeader(uint8_t sender,
                       uint8_t receiver,
                       uint8_t messageType)
{
    return addByte(PACKET_VERSION) &&
           addByte(sender) &&
           addByte(receiver) &&
           addByte(messageType);
}

bool Packet::addHeartbeat(uint16_t uptime)
{
    return addUInt16(uptime);
}

uint16_t Packet::getLength() const
{
    return length;
}

uint8_t* Packet::getData()
{
    return data;
}