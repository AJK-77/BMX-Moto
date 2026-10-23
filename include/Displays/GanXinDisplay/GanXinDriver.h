
#pragma once

#include <Arduino.h>

class GanXinDriver
{
public:
    static constexpr uint8_t DIGITS = 4;
    static constexpr uint8_t OUTPUTS_PER_DIGIT = 16;
    static constexpr uint8_t TOTAL_OUTPUTS =
        DIGITS * OUTPUTS_PER_DIGIT;

    GanXinDriver();

    void begin();

    // Een cijfer instellen op positie 0 t/m 3.
    // Waarde 0 t/m 9; 10 betekent leeg.
    bool setDigit(uint8_t digit, uint8_t value);

    // Alle vier cijfers in één keer instellen.
    // Waarde 10 betekent een lege positie.
    bool setDigits(
        uint8_t digit0,
        uint8_t digit1,
        uint8_t digit2,
        uint8_t digit3
    );

    // Helderheidsstap 1 t/m 9.
    // De uiteindelijke PWM-waarden worden gekalibreerd.
    void setBrightness(uint8_t percent);

private:
    bool outputs[TOTAL_OUTPUTS] = {};
    uint8_t digits[DIGITS] = {12, 12, 12, 12};
    uint8_t brightnessStep = 100;
    bool initialized = false;

    void sendOutputs();
    void renderDigit(uint8_t digit);
    bool renderAllDigits();
    void setSegment(uint8_t digit, char segment, bool enabled);
    void applyBrightness();
};
