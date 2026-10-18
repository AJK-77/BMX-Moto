#pragma once

#include "Handheld/Input/TCA8418.h"

class VolumeScreen
{
public:
    using BackCallback = void (*)();

    VolumeScreen();

    void begin();
    void draw();
    void update();

    void setBackCallback(BackCallback callback);
    void onKeyEvent(const TCA8418::KeyEvent& event);
    uint8_t getVolume() const;

private:
    BackCallback backCallback = nullptr;

    uint8_t volume = 100;

    void drawBar(
        int16_t x,
        int16_t y,
        uint8_t value
    );
};