
#include <Arduino.h>

#include "Displays/GanXinDisplay/GanXinDriver.h"
#include "Displays/GanXinDisplay/Pins.h"

namespace
{
    // Symbolen voor setDigit():
    // 0..9 = cijfers
    // 10   = A
    // 11   = F
    // 12   = blank

    constexpr uint8_t SYMBOL_A     = 10;
    constexpr uint8_t SYMBOL_F     = 11;
    constexpr uint8_t SYMBOL_BLANK = 12;

    // Voorlopige PWM-instellingen.
    // Frequentie en helderheidsmapping moeten nog op hardware
    // worden gekalibreerd.
    constexpr uint32_t OE_PWM_FREQUENCY = 1000;
    constexpr uint8_t OE_PWM_RESOLUTION = 8;
    constexpr uint16_t OE_PWM_MAX = 255;

    bool sameOutputs(
        const bool* first,
        const bool* second,
        uint8_t count)
    {
        for (uint8_t i = 0; i < count; ++i)
        {
            if (first[i] != second[i])
            {
                return false;
            }
        }

        return true;
    }
}

GanXinDriver::GanXinDriver()
{
}

void GanXinDriver::begin()
{
    pinMode(PIN_GX_DATA, OUTPUT);
    pinMode(PIN_GX_CLK, OUTPUT);
    pinMode(PIN_GX_LE, OUTPUT);
    pinMode(PIN_GX_OE, OUTPUT);

    digitalWrite(PIN_GX_DATA, LOW);
    digitalWrite(PIN_GX_CLK, LOW);
    digitalWrite(PIN_GX_LE, LOW);

    // ESP32 Arduino PWM.
    analogWriteResolution(OE_PWM_RESOLUTION);
    analogWriteFrequency(OE_PWM_FREQUENCY);

    initialized = true;

    // Start op maximale helderheid.
    brightnessStep = 100;
    applyBrightness();

    // Start met 0000 op het display.
    digits[0] = 0;
    digits[1] = 0;
    digits[2] = 0;
    digits[3] = 0;

    // Alle segmenten van 0000 naar de hardware sturen.
    renderAllDigits();
}

bool GanXinDriver::setDigit(uint8_t digit, uint8_t value)
{
    if (!initialized || digit >= DIGITS)
    {
        return false;
    }

    // Ondersteun cijfers, A, F en blank.
    if (value > SYMBOL_BLANK)
    {
        value = SYMBOL_BLANK;
    }

    if (digits[digit] == value)
    {
        return false;
    }

    digits[digit] = value;
    return renderAllDigits();
}

bool GanXinDriver::setDigits(
    uint8_t digit0,
    uint8_t digit1,
    uint8_t digit2,
    uint8_t digit3)
{
    if (!initialized)
    {
        return false;
    }

    uint8_t newDigits[DIGITS] =
    {
        digit0, digit1, digit2, digit3
    };

    for (uint8_t i = 0; i < DIGITS; ++i)
    {
        if (newDigits[i] > SYMBOL_BLANK)
        {
            newDigits[i] = SYMBOL_BLANK;
        }
    }

    bool changed = false;

    for (uint8_t i = 0; i < DIGITS; ++i)
    {
        if (digits[i] != newDigits[i])
        {
            digits[i] = newDigits[i];
            changed = true;
        }
    }

    if (!changed)
    {
        return false;
    }

    return renderAllDigits();
    }

void GanXinDriver::setBrightness(uint8_t percent)
{
    if (percent < 10)
    {
        percent = 10;
    }

    if (percent > 100)
    {
        percent = 100;
    }

    if (brightnessStep == percent)
    {
        return;
    }

    brightnessStep = percent;

    if (initialized)
    {
        applyBrightness();
    }
}

void GanXinDriver::applyBrightness()
{
    // OE is actief laag:
    // meer PWM-hoogtijd betekent verder dimmen.
    //
    // Voorlopige lineaire mapping:
    // 10% gewenste helderheid  -> 90% PWM hoog
    // 100% gewenste helderheid ->  0% PWM hoog
    //
    // Dit wordt later met de werkende hardware gekalibreerd.

    uint16_t highPercent = 100 - brightnessStep;

    uint16_t pwmValue =
        (highPercent * OE_PWM_MAX) / 100;

    analogWrite(PIN_GX_OE, pwmValue);
}

bool GanXinDriver::renderAllDigits()
{
    bool previousOutputs[TOTAL_OUTPUTS];

    for (uint8_t i = 0; i < TOTAL_OUTPUTS; ++i)
    {
        previousOutputs[i] = outputs[i];
    }

    for (uint8_t digit = 0; digit < DIGITS; ++digit)
    {
        renderDigit(digit);
    }

    // Geen verzending als het patroon niet veranderd is.
    if (sameOutputs(previousOutputs, outputs, TOTAL_OUTPUTS))
    {
        return false;
    }

    // Nieuw patroon versturen.
    sendOutputs();
    return true;
}

void GanXinDriver::renderDigit(uint8_t digit)
{
    if (digit >= DIGITS)
    {
        return;
    }

    const uint8_t base = digit * OUTPUTS_PER_DIGIT;

    // Eerst alle 16 uitgangen van deze digit uitschakelen.
    for (uint8_t channel = 0;
         channel < OUTPUTS_PER_DIGIT;
         ++channel)
    {
        outputs[base + channel] = false;
    }

    const uint8_t value = digits[digit];

    // Segmenten A t/m G.
    // De driver activeert steeds beide kanalen van een segment.
    switch (value)
    {
        case 0:
            setSegment(digit, 'A', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'C', true);
            setSegment(digit, 'D', true);
            setSegment(digit, 'E', true);
            setSegment(digit, 'F', true);
            break;

        case 1:
            setSegment(digit, 'B', true);
            setSegment(digit, 'C', true);
            break;

        case 2:
            setSegment(digit, 'A', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'G', true);
            setSegment(digit, 'E', true);
            setSegment(digit, 'D', true);
            break;

        case 3:
            setSegment(digit, 'A', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'G', true);
            setSegment(digit, 'C', true);
            setSegment(digit, 'D', true);
            break;

        case 4:
            setSegment(digit, 'F', true);
            setSegment(digit, 'G', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'C', true);
            break;

        case 5:
            setSegment(digit, 'A', true);
            setSegment(digit, 'F', true);
            setSegment(digit, 'G', true);
            setSegment(digit, 'C', true);
            setSegment(digit, 'D', true);
            break;

        case 6:
            setSegment(digit, 'A', true);
            setSegment(digit, 'F', true);
            setSegment(digit, 'G', true);
            setSegment(digit, 'E', true);
            setSegment(digit, 'C', true);
            setSegment(digit, 'D', true);
            break;

        case 7:
            setSegment(digit, 'A', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'C', true);
            break;

        case 8:
            setSegment(digit, 'A', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'C', true);
            setSegment(digit, 'D', true);
            setSegment(digit, 'E', true);
            setSegment(digit, 'F', true);
            setSegment(digit, 'G', true);
            break;

        case 9:
            setSegment(digit, 'A', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'C', true);
            setSegment(digit, 'D', true);
            setSegment(digit, 'F', true);
            setSegment(digit, 'G', true);
            break;

        case SYMBOL_A:
            setSegment(digit, 'A', true);
            setSegment(digit, 'B', true);
            setSegment(digit, 'C', true);
            setSegment(digit, 'E', true);
            setSegment(digit, 'F', true);
            setSegment(digit, 'G', true);
            break;

        case SYMBOL_F:
            setSegment(digit, 'A', true);
            setSegment(digit, 'E', true);
            setSegment(digit, 'F', true);
            setSegment(digit, 'G', true);
            break;

        case SYMBOL_BLANK:
        default:
            // Geen segmenten aan.
            break;
    }
}

void GanXinDriver::setSegment(
    uint8_t digit,
    char segment,
    bool enabled)
{
    if (digit >= DIGITS)
    {
        return;
    }

    uint8_t firstChannel;

    switch (segment)
    {
        case 'A': firstChannel = 0;  break;
        case 'B': firstChannel = 2;  break;
        case 'C': firstChannel = 4;  break;
        case 'D': firstChannel = 6;  break;
        case 'E': firstChannel = 8;  break;
        case 'F': firstChannel = 10; break;
        case 'G': firstChannel = 12; break;

        default:
            return;
    }

    const uint8_t base = digit * OUTPUTS_PER_DIGIT;

    // A1+A2, B1+B2 enzovoort.
    outputs[base + firstChannel] = enabled;
    outputs[base + firstChannel + 1] = enabled;
}

void GanXinDriver::sendOutputs()
{
    // Zelfde overdracht als in de werkende Arduino-test.
    digitalWrite(PIN_GX_LE, LOW);

    for (int i = TOTAL_OUTPUTS - 1; i >= 0; --i)
    {
        digitalWrite(PIN_GX_CLK, LOW);
        digitalWrite(PIN_GX_DATA, outputs[i] ? HIGH : LOW);
        digitalWrite(PIN_GX_CLK, HIGH);
    }

    digitalWrite(PIN_GX_CLK, LOW);

    // Shiftregister naar uitgangslatch kopiëren.
    digitalWrite(PIN_GX_LE, HIGH);
    delayMicroseconds(2);
    digitalWrite(PIN_GX_LE, LOW);
}
