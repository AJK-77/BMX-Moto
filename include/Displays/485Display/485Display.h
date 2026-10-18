#pragma once

#include "Common/LEDManager.h"
#include "Common/RaceState.h"

class Display485
{
public:
    Display485();

    void begin();
    void update();

    void onRFActivity();
    void onHeartbeat();
    void onHeartbeatLost(uint8_t node);
    void onHeartbeatRestored();
    void onHeartbeatReceived(uint16_t eventSequence);
    void onStateChanged();

    void setRaceState(RaceState* state);
    void sendTestFrame();

private:
    LEDManager leds;
    RaceState* raceState = nullptr;
    uint16_t lastHeartbeatEvent = 0;

    void sendDisplayFrame();
};