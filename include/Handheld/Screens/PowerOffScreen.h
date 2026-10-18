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

    void setBackCallback(void (*callback)());
    void setPowerDownCallback(void (*callback)());

    private:
    void (*backCallback)() = nullptr;
    void (*powerDownCallback)() = nullptr;

    bool countdownActive = false;
    uint8_t countdownValue = 10;
    unsigned long countdownStart = 0;

    void startCountdown();
    void cancel();
};