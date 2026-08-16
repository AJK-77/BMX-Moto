#pragma once

#include <stdint.h>

#include "Common/RF/ESPNowDriver.h"
#include "Common/Protocol/Packet.h"
#include "Common/RaceState.h"

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

    void setRaceState(RaceState* state);

private:
    void sendPacket(Packet& packet);
    void activity();

    void processPacket(Packet& packet);

    ESPNowDriver espNow;
    ActivityCallback activityCallback = nullptr;

    RaceState* raceState = nullptr;
};