#include "Common/Protocol/Packet.h"

#include "Common/Constants.h"
#include "Common/Protocol/CRC/CRC.h"


Packet::Packet()
{
    clear();
}


// ============================================================
// Basic packet handling
// ============================================================

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


// ============================================================
// Data adding
// ============================================================

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


// ============================================================
// Header
// ============================================================

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


// ============================================================
// Heartbeat
// ============================================================

bool Packet::addHeartbeat(
    uint16_t eventSequence,
    const char* raceNumber,
    uint8_t mode
)
{
    if (raceNumber == nullptr)
    {
        return false;
    }

    // RaceState is always exactly four characters.
    // Allowed characters are 0-9 and space.
    for (uint8_t i = 0; i < 4; i++)
    {
        const char c = raceNumber[i];

        if (!((c >= '0' && c <= '9') || c == ' '))
        {
            return false;
        }
    }

    if (raceNumber[4] != '\0')
    {
        return false;
    }

    if (!addUInt16(eventSequence))
    {
        return false;
    }

    if (!addByte(static_cast<uint8_t>(raceNumber[0])) ||
        !addByte(static_cast<uint8_t>(raceNumber[1])) ||
        !addByte(static_cast<uint8_t>(raceNumber[2])) ||
        !addByte(static_cast<uint8_t>(raceNumber[3])))
    {
        return false;
    }

    return addByte(mode);
}


// ============================================================
// Event
// ============================================================

bool Packet::addEvent(
    EventType eventType
)
{
    return addByte(static_cast<uint8_t>(eventType));
}

// ============================================================
// Brightness
// ============================================================

bool Packet::addBrightness(uint8_t brightness)
{
    if (brightness > 100)
    {
        return false;
    }

    return addByte(brightness);
}

// ============================================================
// CRC
// ============================================================

bool Packet::addCRC()
{
    if (length + 2 > MAX_PACKET_SIZE)
    {
        return false;
    }

    const uint16_t crc = CRC::calculate(data, length);

    return addUInt16(crc);
}


// ============================================================
// Basic packet information
// ============================================================

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


// ============================================================
// Heartbeat
// ============================================================

uint16_t Packet::getEventSequence() const
{
    // Header       = 4 bytes
    // EventSequence = bytes 4 + 5

    if (length < 6)
    {
        return 0;
    }

    return static_cast<uint16_t>(data[4]) |
           (static_cast<uint16_t>(data[5]) << 8);
}


const char* Packet::getHeartbeatRaceNumber() const
{
    // Header        = 4 bytes
    // EventSequence = 2 bytes
    // RaceNumber    = bytes 6..9

    static char raceNumber[5];

    if (length < 10)
    {
        raceNumber[0] = '\0';
        return raceNumber;
    }

    raceNumber[0] = static_cast<char>(data[6]);
    raceNumber[1] = static_cast<char>(data[7]);
    raceNumber[2] = static_cast<char>(data[8]);
    raceNumber[3] = static_cast<char>(data[9]);
    raceNumber[4] = '\0';

    return raceNumber;
}


uint8_t Packet::getHeartbeatMode() const
{
    // Header        = 4 bytes
    // EventSequence = 2 bytes
    // RaceNumber    = 4 bytes
    // Mode          = byte 10

    if (length < 11)
    {
        return 0;
    }

    return data[10];
}


// ============================================================
// Event
// ============================================================

EventType Packet::getEventType() const
{
    // Header    = 4 bytes
    // EventType = byte 4

    if (length < 5)
    {
        return static_cast<EventType>(0);
    }

    return static_cast<EventType>(data[4]);
}

// ============================================================
// Brightness
// ============================================================

uint8_t Packet::getBrightness() const
{
    // Header    = 4 bytes
    // Brightness = byte 4

    if (length < 5)
    {
        return 0;
    }

    return data[4];
}

// ============================================================
// Raw packet data
// ============================================================

uint8_t* Packet::getData()
{
    return data;
}


const uint8_t* Packet::getData() const
{
    return data;
}