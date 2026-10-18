#include "Handheld/Screens/VolumeScreen.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"

#include <Arduino.h>

VolumeScreen::VolumeScreen()
{
}

void VolumeScreen::begin()
{
     // 0% = MUTE
    volume = 100;
}

void VolumeScreen::draw()
{
    TFT.clear();

    // Titel
    TFT.drawText(
        20,
        30,
        "GELUID",
        2
    );

    // VOLUME
    TFT.drawText(
        135,
        70,
        "VOLUME",
        1
    );

    // Volume balk
    drawBar(
        56,
        100,
        volume
    );

    // Percentage
    char text[8];

    snprintf(
        text,
        sizeof(text),
        "%u%%",
        volume
    );

    TFT.drawText(
        145,
        125,
        text,
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

void VolumeScreen::drawBar(int16_t x, int16_t y, uint8_t value)
{
    constexpr uint8_t BLOCK_COUNT = 10;

    constexpr int16_t BLOCK_WIDTH  = 18;
    constexpr int16_t BLOCK_HEIGHT = 12;
    constexpr int16_t BLOCK_GAP    = 3;

    uint8_t filledBlocks = value / 10;

    if (filledBlocks > BLOCK_COUNT)
        filledBlocks = BLOCK_COUNT;

    for (uint8_t i = 0; i < BLOCK_COUNT; i++)
    {
        const int16_t blockX =
            x + (i * (BLOCK_WIDTH + BLOCK_GAP));

        if (i < filledBlocks)
        {
            // Gevuld blok
            TFT.fillRect(
                blockX,
                y,
                BLOCK_WIDTH,
                BLOCK_HEIGHT,
                TFT_WHITE
            );
        }
        else
        {
            // Leeg blok: alleen rand
            TFT.fillRect(
                blockX,
                y,
                BLOCK_WIDTH,
                2,
                TFT_WHITE
            );

            TFT.fillRect(
                blockX,
                y + BLOCK_HEIGHT - 2,
                BLOCK_WIDTH,
                2,
                TFT_WHITE
            );

            TFT.fillRect(
                blockX,
                y,
                2,
                BLOCK_HEIGHT,
                TFT_WHITE
            );

            TFT.fillRect(
                blockX + BLOCK_WIDTH - 2,
                y,
                2,
                BLOCK_HEIGHT,
                TFT_WHITE
            );
        }
    }
}

void VolumeScreen::update()
{
}

void VolumeScreen::setBackCallback(BackCallback callback)
{
    backCallback = callback;
}

void VolumeScreen::onKeyEvent(
    const TCA8418::KeyEvent& event
)
{
    if (event.type != TCA8418::EventType::DOWN)
    {
        return;
    }

    // Terug
    if (event.key == TCA8418::Key::ESC ||
        event.key == TCA8418::Key::SKL)
    {
        if (backCallback != nullptr)
        {
            backCallback();
        }

        return;
    }

    // Volume lager
    if (event.key == TCA8418::Key::LEFT)
    {
        if (volume > 0)
        {
            volume -= 10;
        }

        draw();
        return;
    }

    // Volume hoger
    if (event.key == TCA8418::Key::RIGHT)
    {
        if (volume < 100)
        {
            volume += 10;
        }

        draw();
        return;
    }
}

uint8_t VolumeScreen::getVolume() const
{
    return volume;
}