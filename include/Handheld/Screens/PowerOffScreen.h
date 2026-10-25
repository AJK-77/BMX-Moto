
#pragma once

#include "Handheld/Input/TCA8418.h"

class PowerOffScreen
{
public:
    PowerOffScreen();

    void begin();
    void draw();
    void update();

    void onKeyEvent(const TCA8418::KeyEvent& event);
    void onPowerButton();

    void setBackCallback(void (*callback)());
    void setPowerDownCallback(void (*callback)());

private:
    void (*backCallback)() = nullptr;
    void (*powerDownCallback)() = nullptr;

    unsigned long firstPowerPress = 0;
    bool waitingForSecondPress = false;

    void cancel();
};
