#pragma once

// ============================================================
// BMX-Moto
// RF Manager
// ============================================================

#include "Common/Protocol/Packet.h"

class RFManager
{
public:
    RFManager();

    void begin();
    void update();

    void send();
    void sendHeartbeat();

private:
    void sendPacket(Packet& packet);
};