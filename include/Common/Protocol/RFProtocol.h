#pragma once

// ============================================================
// BMX-Moto
// RF Protocol
// ============================================================

#include <stdint.h>

#include "Common/Protocol/Packet.h"
#include "Common/Protocol/EventType.h"
#include "Common/RaceState.h"


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
        uint16_t eventSequence,
        const char* raceNumber,
        RaceMode mode
    );

    // --------------------------------------------------------
    // Event
    // --------------------------------------------------------
    static bool createEvent(
        Packet& packet,
        uint8_t sender,
        uint8_t receiver,
        EventType eventType
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