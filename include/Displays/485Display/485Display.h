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
    void onHeartbeatReceived(uint16_t eventSequence);

    void setRaceState(RaceState* state);

private:
    LEDManager leds;
    RaceState* raceState = nullptr;
    uint16_t lastHeartbeatEvent = 0;
};