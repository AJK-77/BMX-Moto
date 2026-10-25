
#include <Arduino.h>

#include "Handheld/Screens/PowerOffScreen.h"
#include "Handheld/GUI/TFT.h"

PowerOffScreen::PowerOffScreen()
{
}

void PowerOffScreen::begin()
{
    waitingForSecondPress = false;
    firstPowerPress = 0;
}


void PowerOffScreen::draw()
{
    TFT.clear();

    TFT.drawText(
        20,
        35,
        "WEET JE HET ZEKER?",
        2
    );

        TFT.drawText(
        205,
        85,
        "2X OFF",
        2
    );

    TFT.drawText(
        25,
        85,
        "ESC IS",
        2
    );

    TFT.drawText(
        35,
        115,
        "VOOR",
        2
    );

    TFT.drawText(
        215,
        115,
        "VOOR",
        2
    );

        TFT.drawText(
        1,
        145,
        "ANNULEREN",
        2
    );


    TFT.drawText(
        180,
        145,
        "UITZETTEN",
        2
    );
}


void PowerOffScreen::update()
{
    // Geen countdown nodig.
}

void PowerOffScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (event.type != TCA8418::EventType::DOWN)
        return;

    if (event.key == TCA8418::Key::ESC)
    {
        cancel();
    }
}

void PowerOffScreen::onPowerButton()
{
    unsigned long now = millis();

    if (!waitingForSecondPress)
    {
        firstPowerPress = now;
        waitingForSecondPress = true;
        return;
    }

    if (now - firstPowerPress <= 1000)
    {
        waitingForSecondPress = false;

        if (powerDownCallback != nullptr)
        {
            powerDownCallback();
        }
    }
    else
    {
        // De vorige druk is verlopen.
        // Deze druk wordt de eerste druk van een nieuwe dubbelklik.
        firstPowerPress = now;
    }
}

void PowerOffScreen::cancel()
{
    waitingForSecondPress = false;
    firstPowerPress = 0;

    if (backCallback != nullptr)
        backCallback();
}

void PowerOffScreen::setBackCallback(void (*callback)())
{
    backCallback = callback;
}

void PowerOffScreen::setPowerDownCallback(void (*callback)())
{
    powerDownCallback = callback;
}
