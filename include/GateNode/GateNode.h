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
    void onHeartbeatReceived(uint16_t eventSequence);
    void setRFManager(RFManager* manager);
    void setRaceState(RaceState* state);

private:
    LEDManager leds;

    bool gateState = false;
    bool lastGateInput = true;

    RFManager* rf = nullptr;
    RaceState* raceState = nullptr;

    uint16_t lastHeartbeatEvent = 0;

    unsigned long lastGateDrop = 0;
};