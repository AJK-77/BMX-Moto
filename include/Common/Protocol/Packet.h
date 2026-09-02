#pragma once

// ============================================================
// BMX-Moto
// RF Packet
// ============================================================

#include <stdint.h>

#include "Common/Protocol/MessageType.h"
#include "Common/Protocol/EventType.h"


class Packet
{
public:
    static constexpr uint16_t MAX_PACKET_SIZE = 64;

    Packet();

    void clear();

    bool load(const uint8_t* data, uint16_t length);

    bool addByte(uint8_t value);
    bool addUInt16(uint16_t value);

    bool addHeader(
        uint8_t sender,
        uint8_t receiver,
        MessageType messageType
    );

    bool addHeartbeat(
    uint16_t eventSequence,
    uint16_t raceNumber,
    uint8_t mode
);

    bool addCRC();

    uint16_t getLength() const;

    uint8_t getSender() const;
    uint8_t getReceiver() const;

    MessageType getMessageType() const;

    uint16_t getEventSequence() const;
    uint16_t getHeartbeatRaceNumber() const;
    uint8_t getHeartbeatMode() const;

    EventType getEventType() const;
    uint16_t getRaceNumber() const;

    uint8_t* getData();
    const uint8_t* getData() const;

private:
    uint8_t data[MAX_PACKET_SIZE];
    uint16_t length = 0;
};