#pragma once

#include <stdint.h>

#include "Common/RF/ESPNowDriver.h"
#include "Common/Protocol/Packet.h"
#include "Common/Protocol/EventType.h"
#include "Common/RaceState.h"

class RFManager
{
public:
    using ActivityCallback = void (*)(void);
    using StateChangeCallback = void (*)(void);

    RFManager();

    bool begin();
    void update();

    void send();
    void sendHeartbeat();
    void sendEvent(EventType eventType);

    void setActivityCallback(ActivityCallback callback);
    void setStateChangeCallback(StateChangeCallback callback);
    void setRaceState(RaceState* state);

private:
    void sendPacket(Packet& packet);
    void processPacket(Packet& packet);
    void activity();

    ESPNowDriver espNow;

    ActivityCallback activityCallback = nullptr;
    StateChangeCallback stateChangeCallback = nullptr;

    RaceState* raceState = nullptr;
};