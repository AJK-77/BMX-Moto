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

    void drawBattery(
    bool led25,
    bool led50,
    bool led75,
    bool led100
);

    void showKey(const char *text);
    void showLine(int y);

    void fillRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t color
);

private:
    TFT_eSPI tft;

    unsigned long splashStart = 0;
};

extern TFTClass TFT;