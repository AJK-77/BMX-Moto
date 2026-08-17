#pragma once

#include "Common/LEDManager.h"

class Display485
{
public:
    Display485();

    void begin();
    void update();

    void onRFActivity();
    void onHeartbeat();
    void onHeartbeatReceived(uint16_t eventSequence);

private:
    LEDManager leds;
    uint16_t lastHeartbeatEvent = 0;
};