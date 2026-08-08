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
                       MessageType messageType)
{
    return addByte(PACKET_VERSION) &&
           addByte(sender) &&
           addByte(receiver) &&
           addByte(static_cast<uint8_t>(messageType));
}
bool Packet::addCRC()
{
    uint16_t crc = 0;

    for (uint16_t i = 0; i < length; i++)
    {
        crc ^= data[i];
    }

    return addUInt16(crc);
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