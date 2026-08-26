#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class TFTClass
{
public:
    bool begin();

    void clear();
    void update();

    void drawText(int16_t x, int16_t y, const char *text, uint8_t size = 1);
    void drawLogo();

    void showSplash();
    bool splashFinished();

    void showKey(const char *text);
    void showLine(int y);

private:
    TFT_eSPI tft;

    unsigned long splashStart = 0;
};

extern TFTClass TFT;