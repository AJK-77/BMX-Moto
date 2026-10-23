#pragma once

#include <Arduino.h>
#include "Common/LEDManager.h"
#include "Common/RaceState.h"
#include "Displays/GanXinDisplay/GanXinDriver.h"

class GanXinDisplay
{
public:
    GanXinDisplay();

    void begin();
    void update();

    void onRFActivity();
    void onHeartbeat();
    void onHeartbeatLost(uint8_t node);
    void onHeartbeatRestored();
    void onHeartbeatReceived(uint16_t eventSequence);
    void onStateChanged();
    void setBrightness(uint8_t brightness);
    void setRaceState(RaceState* state);

private:
    LEDManager leds;
    GanXinDriver driver;

    RaceState* raceState = nullptr;
    uint16_t lastHeartbeatEvent = 0;
};