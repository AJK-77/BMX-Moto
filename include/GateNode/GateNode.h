#pragma once

#include <stdint.h>

#include "Common/LEDManager.h"
#include "Common/RF/RFManager.h"
#include "Common/RaceState.h"

class GateNode
{
public:
    GateNode();

    void begin();
    void update();

    void onRFActivity();
    void onHeartbeat();

    void setRFManager(RFManager* manager);
    void setRaceState(RaceState* state);

private:
    LEDManager leds;

    bool gateState = false;
    bool lastGateInput = true;

    RFManager* rf = nullptr;
    RaceState* raceState = nullptr;

    unsigned long lastGateDrop = 0;
};