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

    // Watchdog foutcode
    void errorCode(uint8_t nodeAddress);
    void clearErrorCode();

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

    // --------------------------------------------------------
    // Watchdog foutcode
    // --------------------------------------------------------

    enum class ErrorState : uint8_t
    {
        Idle,
        LongOn,
        LongOff,
        DigitOn,
        DigitOff,
        DigitPause,
        EndPause
    };

    ErrorState errorState = ErrorState::Idle;

    uint8_t errorNodeAddress = 0;

    uint8_t errorDigits[3] = {};
    uint8_t errorDigitCount = 0;
    uint8_t errorDigitIndex = 0;
    uint8_t errorFlashCount = 0;

    unsigned long errorTimer = 0;

    static constexpr unsigned long ERROR_LONG = 1000;
    static constexpr unsigned long ERROR_SHORT = 200;
    static constexpr unsigned long ERROR_DIGIT_PAUSE = 400;
    static constexpr unsigned long ERROR_END_PAUSE = 1000;

    void updateErrorCode();
};