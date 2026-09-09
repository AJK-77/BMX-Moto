#include "Handheld/Screens/RFScreen.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"

#include <Arduino.h>
#include <string.h>

namespace
{
constexpr int16_t TITLE_Y = GuiLayout::STATUS_HEIGHT + 6;
constexpr int16_t DATA_Y  = GuiLayout::STATUS_HEIGHT + 30;
constexpr int16_t LINE_SPACING = 20;
}

RFScreen::RFScreen()
{
}

void RFScreen::begin()
{
    for (uint8_t i = 0; i < MAX_LINES; i++)
    {
        lines[i][0] = '\0';
    }
}

void RFScreen::draw()
{
    TFT.clear();

    // Status
    TFT.drawText(
        2,
        GuiLayout::STATUS_Y + 5,
        "RF OK",
        1
    );

    // Titel
    TFT.drawText(
        20,
        TITLE_Y,
        "RADIO STATUS",
        1
    );

    // RF-data
    bool hasData = false;

    for (uint8_t i = 0; i < MAX_LINES; i++)
    {
        if (lines[i][0] != '\0')
        {
            hasData = true;

            TFT.drawText(
                20,
                DATA_Y + (i * LINE_SPACING),
                lines[i],
                1
            );
        }
    }

    if (!hasData)
    {
        TFT.drawText(
            20,
            DATA_Y,
            "WACHT OP RF DATA",
            1
        );
    }

    // Softkey
    TFT.drawText(
        GuiLayout::SKL_X,
        GuiLayout::SOFTKEY_Y,
        "TERUG",
        2
    );

    // Batterijsymbool
    TFT.drawBattery(true, true, true, true);
}

void RFScreen::update()
{
    if (!dataChanged)
        return;

    dataChanged = false;
    draw();
}

void RFScreen::setBackCallback(BackCallback callback)
{
    backCallback = callback;
}

void RFScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (event.type != TCA8418::EventType::DOWN)
        return;

    if (event.key == TCA8418::Key::ESC ||
        event.key == TCA8418::Key::SKL)
    {
        if (backCallback != nullptr)
        {
            backCallback();
        }

        return;
    }
}

void RFScreen::showTx(const char* text)
{
    addLine("TX ", text);
}

void RFScreen::showRx(const char* text)
{
    addLine("RX ", text);
}

void RFScreen::addLine(
    const char* prefix,
    const char* text)
{
    if (prefix == nullptr || text == nullptr)
        return;

    // Oude regels één positie naar beneden schuiven
    for (uint8_t i = MAX_LINES - 1; i > 0; i--)
    {
        strcpy(lines[i], lines[i - 1]);
    }

    // Nieuwe regel bovenaan
    snprintf(
        lines[0],
        MAX_LINE_LENGTH + 1,
        "%s%s",
        prefix,
        text
    );

    dataChanged = true;
}