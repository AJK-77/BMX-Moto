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

    // --------------------------------------------------------
    // Heartbeat
    // --------------------------------------------------------
    bool addHeartbeat(
        uint16_t eventSequence,
        const char* raceNumber,
        uint8_t mode
    );

    // --------------------------------------------------------
    // Event
    // --------------------------------------------------------
    bool addEvent(
        EventType eventType
    );

    bool addCRC();

    uint16_t getLength() const;
    uint8_t getBrightness() const;
    uint8_t getSender() const;
    uint8_t getReceiver() const;

    MessageType getMessageType() const;

    // --------------------------------------------------------
    // Heartbeat
    // --------------------------------------------------------
    uint16_t getEventSequence() const;
    const char* getHeartbeatRaceNumber() const;
    uint8_t getHeartbeatMode() const;

    // --------------------------------------------------------
    // Event
    // --------------------------------------------------------
    EventType getEventType() const;

    uint8_t* getData();
    const uint8_t* getData() const;

    // --------------------------------------------------------
    // Brightness
    // --------------------------------------------------------
    bool addBrightness(
        uint8_t brightness
    );

private:
    uint8_t data[MAX_PACKET_SIZE];
    uint16_t length = 0;
};