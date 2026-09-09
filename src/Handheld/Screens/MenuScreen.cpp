#include "Handheld/Screens/MenuScreen.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"

MenuScreen menuScreen;

namespace
{
    const char* menuItems[] =
    {
        "RADIO STATUS",
        "HELDERHEID",
        "GELUID",
        "FIRMWARE",
    };
}

MenuScreen::MenuScreen()
{
}

void MenuScreen::begin()
{
    selectedItem = 0;
}

void MenuScreen::setBackCallback(BackCallback callback)
{
    backCallback = callback;
}

void MenuScreen::setSelectCallback(SelectCallback callback)
{
    selectCallback = callback;
}

void MenuScreen::draw()
{
    TFT.clear();

    TFT.drawText(2, GuiLayout::STATUS_Y + 5, "RF OK", 1);

    TFT.drawText(20, 30, "MENU", 2);

    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++)
    {
        drawMenuItem(i);
    }

    TFT.drawText(20, 166, "TERUG", 2);

    TFT.drawBattery(true, true, true, true);
}

void MenuScreen::drawMenuItem(uint8_t index)
{
    const uint16_t y = 70 + (index * 14);

    if (index == selectedItem)
    {
        TFT.drawText(20, y, ">", 1);
        TFT.drawText(38, y, menuItems[index], 1);
    }
    else
    {
        TFT.drawText(20, y, " ", 2);
        TFT.drawText(38, y, menuItems[index], 1);
    }
}

void MenuScreen::update()
{
}

void MenuScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (event.type != TCA8418::EventType::DOWN)
        return;

    if (event.key == TCA8418::Key::UP)
    {
        if (selectedItem > 0)
            selectedItem--;

        draw();
        return;
    }

    if (event.key == TCA8418::Key::DOWN)
    {
        if (selectedItem < MENU_ITEM_COUNT - 1)
            selectedItem++;

        draw();
        return;
    }

        if (event.key == TCA8418::Key::OK)
        {
            if (selectCallback != nullptr)
                selectCallback(selectedItem);

            return;
        }

    if (event.key == TCA8418::Key::ESC ||
        event.key == TCA8418::Key::SKL)
    {
        if (backCallback != nullptr)
            backCallback();

        return;
    }

    Serial.printf("MenuScreen KEY: %s\n", tca8418.keyName(event.key));
}