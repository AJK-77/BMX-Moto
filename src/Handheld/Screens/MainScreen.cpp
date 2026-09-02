#include <Arduino.h>

#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"
#include "Handheld/Screens/MainScreen.h"


MainScreen::MainScreen()
{
}


void MainScreen::begin()
{
}


void MainScreen::setRaceState(RaceState* state)
{
    raceState = state;
}


void MainScreen::draw()
{
    TFT.clear();

    TFT.drawText(2, GuiLayout::STATUS_Y + 5, "RF OK", 1);

    TFT.drawText(25, GuiLayout::RACE_TOP, "MANCHE", 2);
    TFT.drawText(215, GuiLayout::RACE_TOP, "MOTO", 2);

    uint16_t raceNumber = 0;

    if (raceState != nullptr)
    {
        raceNumber = raceState->getRaceNumber();
    }

    uint16_t manche = raceNumber / 1000;
    uint16_t moto = raceNumber % 1000;

    char mancheText[5];
    char motoText[4];

    snprintf(mancheText, sizeof(mancheText), "%u", manche);
    snprintf(motoText, sizeof(motoText), "%03u", moto);

    TFT.drawText(55, 60, mancheText, 6);
    TFT.drawText(170, 60, motoText, 6);

    TFT.drawText(20, 166, "MENU", 2);

    if (raceState != nullptr &&
        raceState->getMode() == RaceMode::AUTO)
    {
        TFT.drawText(240, 166, "AUTO", 2);
    }
    else
    {
        TFT.drawText(240, 166, "MAN", 2);
    }

    TFT.drawBattery(true, true, true, true);
}


void MainScreen::update()
{
}


void MainScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (event.type != TCA8418::EventType::DOWN)
    {
        return;
    }

    Serial.printf(
        "MainScreen KEY: %s\n",
        tca8418.keyName(event.key)
    );

    if (raceState == nullptr)
    {
        return;
    }


    // ---------------------------------------------------------
    // +
    // Increase moto.
    // Works in both MANUAL and AUTO.
    // ---------------------------------------------------------

    if (event.key == TCA8418::Key::PLUS)
    {
        uint16_t raceNumber = raceState->getRaceNumber();

        uint16_t manche = raceNumber / 1000;
        uint16_t moto = raceNumber % 1000;

        moto++;

        if (moto > 999)
        {
            moto = 0;
        }

        raceState->setRaceNumber(
            (manche * 1000) + moto
        );

        raceState->nextEvent();

        draw();
        return;
    }


    // ---------------------------------------------------------
    // -
    // Decrease moto.
    // Works in both MANUAL and AUTO.
    // ---------------------------------------------------------

    if (event.key == TCA8418::Key::MINUS)
    {
        uint16_t raceNumber = raceState->getRaceNumber();

        uint16_t manche = raceNumber / 1000;
        uint16_t moto = raceNumber % 1000;

        if (moto == 0)
        {
            moto = 999;
        }
        else
        {
            moto--;
        }

        raceState->setRaceNumber(
            (manche * 1000) + moto
        );

        raceState->nextEvent();

        draw();
        return;
    }


    // ---------------------------------------------------------
    // SKR
    // Toggle MANUAL / AUTO.
    // ---------------------------------------------------------

    if (event.key == TCA8418::Key::SKR)
    {
        if (raceState->getMode() == RaceMode::AUTO)
        {
            raceState->setMode(RaceMode::MANUAL);
        }
        else
        {
            raceState->setMode(RaceMode::AUTO);
        }

        raceState->nextEvent();

        draw();
        return;
    }
}