#pragma once

#include "Common/LEDManager.h"

class GateNode
{
public:
    GateNode();

    void begin();
    void update();

    void onRFActivity();
    void onHeartbeat();

private:
    LEDManager leds;

    bool gateState;
};