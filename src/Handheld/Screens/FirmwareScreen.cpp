#include "Handheld/Screens/FirmwareScreen.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"

FirmwareScreen::FirmwareScreen()
{
}

void FirmwareScreen::begin()
{
}

void FirmwareScreen::draw()
{
    TFT.clear();

    // Titel
    TFT.drawText(
        20,
        30,
        "FIRMWARE",
        2
    );

    // Tijdelijke inhoud
    TFT.drawText(
        20,
        70,
        "FIRMWARE",
        1
    );

    // Softkey
    TFT.drawText(
        GuiLayout::SKL_X,
        GuiLayout::SOFTKEY_Y,
        "TERUG",
        2
    );
}

void FirmwareScreen::update()
{
}

void FirmwareScreen::setBackCallback(BackCallback callback)
{
    backCallback = callback;
}

void FirmwareScreen::onKeyEvent(
    const TCA8418::KeyEvent& event
)
{
    if (event.type != TCA8418::EventType::DOWN)
    {
        return;
    }

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