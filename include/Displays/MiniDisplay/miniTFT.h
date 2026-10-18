#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class MiniTFTClass
{
public:
    bool begin();

    void clear();

    void showSplash();
    bool splashFinished();

    void drawText(
        int16_t x,
        int16_t y,
        const char* text,
        uint8_t size = 1
    );

private:
    TFT_eSPI tft;

    unsigned long splashStart = 0;
};

extern MiniTFTClass MiniTFT;