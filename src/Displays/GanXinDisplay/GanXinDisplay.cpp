
#include <Arduino.h>
#include "Displays/GanXinDisplay/GanXinDisplay.h"
#include "Displays/GanXinDisplay/Pins.h"

GanXinDisplay::GanXinDisplay()
    : leds(
        PIN_LED_RED,
        PIN_LED_BLUE,
        PIN_LED_YELLOW,
        PIN_LED_GREEN
      )
{
}

void GanXinDisplay::begin()
{
    leds.begin();
    leds.bootFlash();

    // GanXin display-driver initialiseren
    driver.begin();
    driver.setBrightness(100);

    // Display node actief
    leds.green(true);

    Serial.println("GanXin: initialized");
}

void GanXinDisplay::update()
{
    leds.update();

    // GanXin hardware-aansturing komt hier.
}

void GanXinDisplay::onRFActivity()
{
    leds.blueFlash();
}

void GanXinDisplay::onHeartbeat()
{
    leds.greenHeartbeat();
}

void GanXinDisplay::onHeartbeatLost(uint8_t node)
{
    leds.errorCode(node);
}

void GanXinDisplay::onHeartbeatRestored()
{
    leds.clearErrorCode();
}

void GanXinDisplay::onHeartbeatReceived(uint16_t eventSequence)
{
    lastHeartbeatEvent = eventSequence;

    if (raceState != nullptr)
    {
        raceState->setEventSequence(eventSequence);
    }
}

void GanXinDisplay::setRaceState(RaceState* state)
{
    raceState = state;
}

void GanXinDisplay::onStateChanged()
{
    if (raceState == nullptr)
    {
        return;
    }

    const char* raceNumber = raceState->getRaceNumber();

    if (raceNumber == nullptr)
    {
        return;
    }

    uint8_t digits[4];

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (raceNumber[i] >= '0' && raceNumber[i] <= '9')
        {
            digits[i] = raceNumber[i] - '0';
        }
        else
        {
            // Spatie wordt een lege digit.
            digits[i] = 12;
        }
    }

    if (driver.setDigits(digits[0], digits[1], digits[2], digits[3]))
    {
        leds.yellowFlash();
    }
}

void GanXinDisplay::setBrightness(uint8_t brightness)
{
    driver.setBrightness(brightness);
}