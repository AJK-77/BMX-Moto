#pragma once

// ============================================================
// BMX-Moto
// RF Packet
// ============================================================

#include <stdint.h>

#include "Common/Protocol/MessageType.h"

class Packet
{
public:
    Packet();

    void clear();

    bool addByte(uint8_t value);
    bool addUInt16(uint16_t value);

    bool addHeader(uint8_t sender,
                   uint8_t receiver,
                   MessageType messageType);

    bool addHeartbeat(uint16_t uptime);

    bool addCRC();
    
    uint16_t getLength() const;
    uint8_t* getData();



private:
    static constexpr uint16_t MAX_PACKET_SIZE = 64;

    uint8_t data[MAX_PACKET_SIZE];
    uint16_t length = 0;
};