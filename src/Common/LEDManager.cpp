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