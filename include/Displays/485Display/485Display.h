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

private:
    LEDManager leds;
};