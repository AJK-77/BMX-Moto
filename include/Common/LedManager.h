#pragma once

#include <Arduino.h>

class LEDManager
{
public:
    LEDManager(
        uint8_t redPin,
        uint8_t bluePin,
        uint8_t yellowPin,
        uint8_t greenPin
    );

    void begin();
    void update();
    void bootFlash();

    void red(bool on);
    void blue(bool on);
    void yellow(bool on);
    void green(bool on);

    void redFlash();
    void blueFlash();
    void yellowFlash();
    void greenFlash();

    // Groen normaal aan, tijdens heartbeat kort uit
    void greenHeartbeat();

private:
    static constexpr unsigned long FLASH_DURATION = 50;

    uint8_t redPin;
    uint8_t bluePin;
    uint8_t yellowPin;
    uint8_t greenPin;

    bool redFlashActive = false;
    bool blueFlashActive = false;
    bool yellowFlashActive = false;
    bool greenFlashActive = false;
    bool greenHeartbeatActive = false;

    unsigned long redFlashUntil = 0;
    unsigned long blueFlashUntil = 0;
    unsigned long yellowFlashUntil = 0;
    unsigned long greenFlashUntil = 0;
    unsigned long greenHeartbeatUntil = 0;

    void setLed(uint8_t pin, bool on);
};