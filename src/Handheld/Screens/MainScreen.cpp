#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"
#include "Handheld/Screens/MainScreen.h"
#include "Handheld/Screens/MenuScreen.h"

MainScreen::MainScreen()
{
}

void MainScreen::begin()
{
}

void MainScreen::draw()
{
    TFT.clear();

    TFT.drawText(2, GuiLayout::STATUS_Y + 5, "RF OK", 1);

    TFT.drawText(25, GuiLayout::RACE_TOP, "MANCHE", 2);
    TFT.drawText(215, GuiLayout::RACE_TOP, "MOTO", 2);

    TFT.drawText(55, 60, "0", 6);
    TFT.drawText(170, 60, "000", 6);

    TFT.drawText(20, 166, "MENU", 2);
    TFT.drawText(240, 166, "AUTO", 2);

    TFT.drawBattery(true, true, true, true);
}

void MainScreen::update()
{
}

void MainScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (event.type != TCA8418::EventType::DOWN)
        return;

    Serial.printf("MainScreen KEY: %s\n", tca8418.keyName(event.key));
}