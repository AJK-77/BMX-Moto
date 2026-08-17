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
    using HeartbeatRxCallback = void (*)(uint16_t eventSequence);

    RFManager();

    bool begin();
    void update();

    void send();
    void sendHeartbeat();
    void sendEvent(EventType eventType);

    void setActivityCallback(ActivityCallback callback);
    void setHeartbeatRxCallback(HeartbeatRxCallback callback);

    void setRaceState(RaceState* state);

private:
    void sendPacket(Packet& packet);
    void processPacket(Packet& packet);
    void activity();

    ESPNowDriver espNow;

    ActivityCallback activityCallback = nullptr;
    HeartbeatRxCallback heartbeatRxCallback = nullptr;

    RaceState* raceState = nullptr;

    bool firstHeartbeat = true;
};