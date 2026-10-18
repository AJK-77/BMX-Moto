#include "Common/LEDManager.h"

#include <Arduino.h>

LEDManager::LEDManager(
    uint8_t redPin,
    uint8_t bluePin,
    uint8_t yellowPin,
    uint8_t greenPin
)
    : redPin(redPin),
      bluePin(bluePin),
      yellowPin(yellowPin),
      greenPin(greenPin)
{
}

void LEDManager::begin()
{
    pinMode(redPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    // Alle LEDs uit bij opstarten
    red(false);
    blue(false);
    yellow(false);
    green(false);

    redFlashActive = false;
    blueFlashActive = false;
    yellowFlashActive = false;
    greenFlashActive = false;
    greenHeartbeatActive = false;
}

void LEDManager::update()
{
    uint32_t now = millis();

    if (redFlashActive && (int32_t)(now - redFlashUntil) >= 0)
    {
        red(false);
        redFlashActive = false;
    }

    if (blueFlashActive && (int32_t)(now - blueFlashUntil) >= 0)
    {
        blue(false);
        blueFlashActive = false;
    }

    if (yellowFlashActive && (int32_t)(now - yellowFlashUntil) >= 0)
    {
        yellow(false);
        yellowFlashActive = false;
    }

    if (greenFlashActive && (int32_t)(now - greenFlashUntil) >= 0)
    {
        green(false);
        greenFlashActive = false;
    }

    if (greenHeartbeatActive &&
        (int32_t)(now - greenHeartbeatUntil) >= 0)
    {
        green(true);
        greenHeartbeatActive = false;
    }

        updateErrorCode();
}

void LEDManager::red(bool on)
{
    setLed(redPin, on);
}

void LEDManager::blue(bool on)
{
    setLed(bluePin, on);
}

void LEDManager::yellow(bool on)
{
    setLed(yellowPin, on);
}

void LEDManager::green(bool on)
{
    setLed(greenPin, on);
}

void LEDManager::redFlash()
{
    red(true);
    redFlashActive = true;
    redFlashUntil = millis() + FLASH_DURATION;
}

void LEDManager::blueFlash()
{
    blue(true);
    blueFlashActive = true;
    blueFlashUntil = millis() + FLASH_DURATION;
}

void LEDManager::yellowFlash()
{
    yellow(true);
    yellowFlashActive = true;
    yellowFlashUntil = millis() + FLASH_DURATION;
}

void LEDManager::greenFlash()
{
    green(true);
    greenFlashActive = true;
    greenFlashUntil = millis() + FLASH_DURATION;
}

void LEDManager::greenHeartbeat()
{
    green(false);
    greenHeartbeatActive = true;
    greenHeartbeatUntil = millis() + FLASH_DURATION;
}

void LEDManager::setLed(uint8_t pin, bool on)
{
    digitalWrite(pin, on ? HIGH : LOW);
}

void LEDManager::bootFlash()
{
    red(true);
    delay(250);
    red(false);

    blue(true);
    delay(250);
    blue(false);

    yellow(true);
    delay(250);
    yellow(false);

    green(true);
    delay(250);
    green(false);
}

void LEDManager::errorCode(uint8_t nodeAddress)
{
    if (nodeAddress == 0)
    {
        return;
    }

    errorNodeAddress = nodeAddress;

    errorDigitCount = 0;

    if (nodeAddress >= 100)
    {
        errorDigits[errorDigitCount++] = nodeAddress / 100;
        nodeAddress %= 100;
    }

    if (nodeAddress >= 10)
    {
        errorDigits[errorDigitCount++] = nodeAddress / 10;
        nodeAddress %= 10;
    }

    errorDigits[errorDigitCount++] = nodeAddress;

    errorDigitIndex = 0;
    errorFlashCount = 0;

    // Foutcode gebruikt de rode LED
    red(false);

    errorState = ErrorState::LongOn;
    errorTimer = millis() + ERROR_LONG;

    red(true);
}

void LEDManager::clearErrorCode()
{
    errorState = ErrorState::Idle;

    errorNodeAddress = 0;
    errorDigitCount = 0;
    errorDigitIndex = 0;
    errorFlashCount = 0;

    red(false);
}

void LEDManager::updateErrorCode()
{
    if (errorState == ErrorState::Idle)
    {
        return;
    }

    const unsigned long now = millis();

    if ((int32_t)(now - errorTimer) < 0)
    {
        return;
    }

    switch (errorState)
    {
        case ErrorState::LongOn:
            red(false);
            errorTimer = now + ERROR_LONG;
            errorState = ErrorState::LongOff;
            break;

        case ErrorState::LongOff:
            errorFlashCount = 0;
            errorTimer = now + ERROR_SHORT;
            errorState = ErrorState::DigitOn;
            red(true);
            break;

        case ErrorState::DigitOn:
            red(false);

            errorFlashCount++;

            if (errorFlashCount >= errorDigits[errorDigitIndex])
            {
                if (errorDigitIndex + 1 >= errorDigitCount)
                {
                    // Laatste cijfer klaar
                    errorTimer = now + ERROR_SHORT;
                    errorState = ErrorState::EndPause;
                }
                else
                {
                    // Naar volgend cijfer
                    errorDigitIndex++;
                    errorFlashCount = 0;
                    errorTimer = now + ERROR_DIGIT_PAUSE;
                    errorState = ErrorState::DigitPause;
                }
            }
            else
            {
                errorTimer = now + ERROR_SHORT;
                errorState = ErrorState::DigitOff;
            }
            break;

        case ErrorState::DigitOff:
            red(true);
            errorTimer = now + ERROR_SHORT;
            errorState = ErrorState::DigitOn;
            break;

        case ErrorState::DigitPause:
            errorTimer = now + ERROR_SHORT;
            errorState = ErrorState::DigitOn;
            red(true);
            break;

        case ErrorState::EndPause:
            // Meteen opnieuw beginnen met de lange rode puls
            red(false);
            errorDigitIndex = 0;
            errorFlashCount = 0;
            errorTimer = now + ERROR_END_PAUSE;
            errorState = ErrorState::LongOn;
            //red(true);
            break;

        case ErrorState::Idle:
            break;
    }
}