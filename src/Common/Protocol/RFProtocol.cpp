#include "Common/Protocol/RFProtocol.h"

#include "Common/Constants.h"
#include "Common/Protocol/CRC/CRC.h"


// ============================================================
// Heartbeat
// ============================================================

bool RFProtocol::createHeartbeat(
    Packet& packet,
    uint8_t sender,
    uint16_t eventSequence,
    uint16_t raceNumber,
    RaceMode mode
)
{
    packet.clear();

    if (!packet.addHeader(
            sender,
            ADDRESS_BROADCAST,
            MessageType::Heartbeat))
    {
        return false;
    }

    if (!packet.addHeartbeat(
            eventSequence,
            raceNumber,
            static_cast<uint8_t>(mode)))
    {
        return false;
    }

    return packet.addCRC();
}


// ============================================================
// Event
// ============================================================

bool RFProtocol::createEvent(
    Packet& packet,
    uint8_t sender,
    uint8_t receiver,
    uint16_t eventSequence,
    EventType eventType,
    uint16_t raceNumber
)
{
    packet.clear();

    if (!packet.addHeader(
            sender,
            receiver,
            MessageType::Event))
    {
        return false;
    }

    if (!packet.addUInt16(eventSequence))
    {
        return false;
    }

    if (!packet.addByte(static_cast<uint8_t>(eventType)))
    {
        return false;
    }

    if (!packet.addUInt16(raceNumber))
    {
        return false;
    }

    return packet.addCRC();
}


// ============================================================
// Status
// ============================================================

bool RFProtocol::createStatus(
    Packet& packet,
    uint8_t sender,
    uint8_t receiver,
    uint16_t eventSequence
)
{
    packet.clear();

    if (!packet.addHeader(
            sender,
            receiver,
            MessageType::Status))
    {
        return false;
    }

    if (!packet.addUInt16(eventSequence))
    {
        return false;
    }

    return packet.addCRC();
}


// ============================================================
// Validation
// ============================================================

bool RFProtocol::validatePacket(
    const Packet& packet
)
{
    const uint16_t length = packet.getLength();

    if (length < 6)
    {
        return false;
    }

    const uint8_t* data = packet.getData();

    const uint16_t receivedCRC =
        static_cast<uint16_t>(data[length - 2]) |
        (static_cast<uint16_t>(data[length - 1]) << 8);

    const uint16_t calculatedCRC =
        CRC::calculate(data, length - 2);

    return receivedCRC == calculatedCRC;
}