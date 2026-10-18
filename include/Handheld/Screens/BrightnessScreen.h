#pragma once

#include "Handheld/Input/TCA8418.h"

class RFManager;

class BrightnessScreen
{
public:
    using BackCallback = void (*)();

    BrightnessScreen();

    void begin();
    void draw();
    void update();

    void setBackCallback(BackCallback callback);
    void setRFManager(RFManager* manager);

    void onKeyEvent(const TCA8418::KeyEvent& event);

private:
    BackCallback backCallback = nullptr;
    RFManager* rfManager = nullptr;

    uint8_t handheldBrightness = 100;
    uint8_t displayBrightness = 100;

    uint8_t selectedItem = 0;

    void drawBrightnessItem(
        uint8_t index,
        const char* label,
        uint8_t value
    );

    void drawBar(
        int16_t x,
        int16_t y,
        uint8_t value
    );
};