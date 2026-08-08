#include "Common/Protocol/Packet.h"

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

uint16_t Packet::getLength() const
{
    return length;
}

uint8_t* Packet::getData()
{
    return data;
}

bool Packet::addHeader(uint8_t sender,
                       uint8_t receiver,
                       uint8_t messageType)
{
    return addByte(sender) &&
           addByte(receiver) &&
           addByte(messageType);
}