#pragma once

#include <stdint.h>

#include "Common/RF/ESPNowDriver.h"
#include "Common/Protocol/Packet.h"

class RFManager
{
public:
    RFManager();

    bool begin();
    void update();

    void send();
    void sendHeartbeat();

private:
    void sendPacket(Packet& packet);

    ESPNowDriver espNow;
};