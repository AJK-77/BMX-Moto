#include <Arduino.h>

#include "Handheld/Screens/PowerOffScreen.h"
#include "Handheld/GUI/TFT.h"
#include "esp_sleep.h"

PowerOffScreen::PowerOffScreen()
{
}

void PowerOffScreen::begin()
{
    countdownActive = false;
    countdownValue = 10;
}

void PowerOffScreen::draw()
{
    TFT.clear();

    if (!countdownActive)
    {
        TFT.drawText(
            20,
            60,
            "WEET JE HET ZEKER?",
            2
        );

        TFT.drawText(
            20,
            120,
            "ESC = NEE",
            2
        );

        TFT.drawText(
            190,
            120,
            "OK = JA",
            2
        );
    }
    else
    {
        char text[8];

        snprintf(
            text,
            sizeof(text),
            "%u",
            countdownValue
        );

        TFT.drawText(
            70,
            30,
            "UITSCHAKELEN",
            2
        );

        TFT.drawText(
            135,
            70,
            text,
            6
        );

        TFT.drawText(
            105,
            170,
            "ESC = ANNULEREN",
            1
        );
    }
}

void PowerOffScreen::update()
{
    if (!countdownActive)
        return;

    unsigned long now = millis();

    if (now - countdownStart >= 1000)
    {
        countdownStart = now;

        if (countdownValue > 0)
        {
            countdownValue--;
            draw();
        }

        if (countdownValue == 0)
        {
            TFT.setBacklight(0);

            if (powerDownCallback != nullptr)
            {
                powerDownCallback();
            }
        }
    }
}

void PowerOffScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (event.type != TCA8418::EventType::DOWN)
        return;

    if (!countdownActive)
    {
        if (event.key == TCA8418::Key::ESC)
        {
            cancel();
        }
        else if (event.key == TCA8418::Key::OK)
        {
            startCountdown();
        }
    }
    else
    {
        if (event.key == TCA8418::Key::ESC)
        {
            cancel();
        }
    }
}

void PowerOffScreen::startCountdown()
{
    countdownActive = true;
    countdownValue = 10;
    countdownStart = millis();

    draw();
}

void PowerOffScreen::cancel()
{
    countdownActive = false;
    countdownValue = 10;

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