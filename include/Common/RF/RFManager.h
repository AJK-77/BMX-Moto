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
    using RFDataCallback = void (*)(const char* text, bool tx);
    using HeartbeatReceivedCallback = void (*)(uint8_t sender);

    RFManager();

    bool begin();
    void update();
    void shutdown();

    void send();
    void sendHeartbeat();
    void sendEvent(EventType eventType);
    void sendBrightness(uint8_t brightness);

    void setActivityCallback(ActivityCallback callback);
    void setStateChangeCallback(StateChangeCallback callback);
    void setRFDataCallback(RFDataCallback callback);
    void setHeartbeatReceivedCallback(HeartbeatReceivedCallback callback);
    void setRaceState(RaceState* state);

private:
    void sendPacket(Packet& packet);
    void processPacket(Packet& packet);
    void activity();

    ESPNowDriver espNow;

    ActivityCallback activityCallback = nullptr;
    StateChangeCallback stateChangeCallback = nullptr;
    RFDataCallback rfDataCallback = nullptr;
    HeartbeatReceivedCallback heartbeatReceivedCallback = nullptr;

    RaceState* raceState = nullptr;
};