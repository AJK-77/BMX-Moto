#pragma once

// ============================================================
// BMX-Moto
// RF Protocol
// ============================================================

#include <stdint.h>

#include "Common/Protocol/Packet.h"
#include "Common/Protocol/EventType.h"

// ============================================================
// RF Protocol
// ============================================================

class RFProtocol
{
public:
    // --------------------------------------------------------
    // Heartbeat
    // --------------------------------------------------------
    static bool createHeartbeat(
        Packet& packet,
        uint8_t sender,
        uint16_t eventSequence
    );

    // --------------------------------------------------------
    // Event
    // --------------------------------------------------------
    static bool createEvent(
        Packet& packet,
        uint8_t sender,
        uint8_t receiver,
        uint16_t eventSequence,
        EventType eventType,
        uint16_t raceNumber
    );

    // --------------------------------------------------------
    // Status
    // --------------------------------------------------------
    static bool createStatus(
        Packet& packet,
        uint8_t sender,
        uint8_t receiver,
        uint16_t eventSequence
    );

    // --------------------------------------------------------
    // Validation
    // --------------------------------------------------------
    static bool validatePacket(
        const Packet& packet
    );
};