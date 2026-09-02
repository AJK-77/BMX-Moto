#include "Common/Protocol/Packet.h"

#include "Common/Constants.h"
#include "Common/Protocol/CRC/CRC.h"


Packet::Packet()
{
    clear();
}

bool Packet::load(const uint8_t* source, uint16_t sourceLength)
{
    if (source == nullptr)
    {
        return false;
    }

    if (sourceLength == 0 || sourceLength > MAX_PACKET_SIZE)
    {
        return false;
    }

    for (uint16_t i = 0; i < sourceLength; i++)
    {
        data[i] = source[i];
    }

    length = sourceLength;

    return true;
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
    if (length + 2 > MAX_PACKET_SIZE)
    {
        return false;
    }

    data[length++] = static_cast<uint8_t>(value & 0xFF);
    data[length++] = static_cast<uint8_t>((value >> 8) & 0xFF);

    return true;
}


bool Packet::addHeader(
    uint8_t sender,
    uint8_t receiver,
    MessageType messageType
)
{
    return addByte(PACKET_VERSION) &&
           addByte(sender) &&
           addByte(receiver) &&
           addByte(static_cast<uint8_t>(messageType));
}


bool Packet::addHeartbeat(
    uint16_t eventSequence,
    uint16_t raceNumber,
    uint8_t mode
)
{
    return addUInt16(eventSequence) &&
           addUInt16(raceNumber) &&
           addByte(mode);
}


bool Packet::addCRC()
{
    if (length + 2 > MAX_PACKET_SIZE)
    {
        return false;
    }

    const uint16_t crc = CRC::calculate(data, length);

    return addUInt16(crc);
}


uint16_t Packet::getLength() const
{
    return length;
}


uint8_t Packet::getSender() const
{
    if (length < 2)
    {
        return 0;
    }

    return data[1];
}


uint8_t Packet::getReceiver() const
{
    if (length < 3)
    {
        return 0;
    }

    return data[2];
}


MessageType Packet::getMessageType() const
{
    if (length < 4)
    {
        return static_cast<MessageType>(0);
    }

    return static_cast<MessageType>(data[3]);
}

uint16_t Packet::getEventSequence() const
{
    // Header = 4 bytes
    // Event sequence = bytes 4 + 5

    if (length < 6)
    {
        return 0;
    }

    return static_cast<uint16_t>(data[4]) |
           (static_cast<uint16_t>(data[5]) << 8);
}

uint16_t Packet::getHeartbeatRaceNumber() const
{
    // Header       = 4 bytes
    // EventSequence = 2 bytes
    // RaceNumber    = bytes 6 + 7

    if (length < 8)
    {
        return 0;
    }

    return static_cast<uint16_t>(data[6]) |
           (static_cast<uint16_t>(data[7]) << 8);
}

uint8_t Packet::getHeartbeatMode() const
{
    if (length < 9)
    {
        return 0;
    }

    return data[8];
}

EventType Packet::getEventType() const
{
    // Header = 4 bytes
    // Event sequence = 2 bytes
    // Event type = byte 6

    if (length < 7)
    {
        return static_cast<EventType>(0);
    }

    return static_cast<EventType>(data[6]);
}


uint16_t Packet::getRaceNumber() const
{
    // Header       = 4 bytes
    // EventSequence = 2 bytes
    // EventType     = 1 byte
    // RaceNumber    = bytes 7 + 8

    if (length < 9)
    {
        return 0;
    }

    return static_cast<uint16_t>(data[7]) |
           (static_cast<uint16_t>(data[8]) << 8);
}


uint8_t* Packet::getData()
{
    return data;
}


const uint8_t* Packet::getData() const
{
    return data;
}