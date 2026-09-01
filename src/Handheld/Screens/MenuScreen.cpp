#include "Handheld/Screens/MenuScreen.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"

MenuScreen menuScreen;

MenuScreen::MenuScreen()
{
}

void MenuScreen::begin()
{
}

void MenuScreen::draw()
{
    TFT.clear();

    TFT.drawText(2, GuiLayout::STATUS_Y + 5, "RF OK", 1);

    TFT.drawText(20, 30, "MENU", 3);

    TFT.drawText(20, 166, "TERUG", 2);

    TFT.drawBattery(true, true, true, true);
}

void MenuScreen::update()
{
}

void MenuScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (event.type != TCA8418::EventType::DOWN)
        return;

    Serial.printf("MenuScreen KEY: %s\n", tca8418.keyName(event.key));
}