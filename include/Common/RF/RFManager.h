#pragma once

#include <stdint.h>

#include "Common/RF/ESPNowDriver.h"
#include "Common/Protocol/Packet.h"

class RFManager
{
public:
    using ActivityCallback = void (*)(void);

    RFManager();

    bool begin();
    void update();

    void send();
    void sendHeartbeat();

    void setActivityCallback(ActivityCallback callback);

private:
    void sendPacket(Packet& packet);
    void activity();

    ESPNowDriver espNow;
    ActivityCallback activityCallback = nullptr;
};