#include "Handheld/Screens/BrightnessScreen.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"
#include "Common/RF/RFManager.h"


BrightnessScreen::BrightnessScreen()
{
}

void BrightnessScreen::begin()
{
    handheldBrightness = 100;
    displayBrightness = 100;
    selectedItem = 0;
}

void BrightnessScreen::setBackCallback(BackCallback callback)
{
    backCallback = callback;
}

void BrightnessScreen::setRFManager(RFManager* manager)
{
    rfManager = manager;
}

void BrightnessScreen::draw()
{
    TFT.clear();

    // Titel
    TFT.drawText(
        20,
        30,
        "HELDERHEID",
        2
    );

    // Handheld
    drawBrightnessItem(
        0,
        "HANDHELD",
        handheldBrightness
    );

    // Display(s)
    drawBrightnessItem(
        1,
        "DISPLAYS",
        displayBrightness
    );

    // Softkey
    TFT.drawText(
        GuiLayout::SKL_X,
        GuiLayout::SOFTKEY_Y,
        "TERUG",
        2
    );
}

void BrightnessScreen::drawBrightnessItem(
    uint8_t index,
    const char* label,
    uint8_t value
)
{
    const int16_t labelY = 65 + (index * 55);
    const int16_t barY   = 82 + (index * 55);

    // Selectie-indicator
    if (index == selectedItem)
    {
        TFT.drawText(
            20,
            labelY,
            ">",
            1
        );
    }
    else
    {
        TFT.drawText(
            20,
            labelY,
            " ",
            1
        );
    }

    // Label
    TFT.drawText(
        38,
        labelY,
        label,
        1
    );

    // Balk
    drawBar(
        38,
        barY,
        value
    );

    // Percentage
    char text[8];

    snprintf(
        text,
        sizeof(text),
        "%u%%",
        value
    );

    TFT.drawText(
        260,
        barY,
        text,
        1
    );
}

void BrightnessScreen::drawBar(
    int16_t x,
    int16_t y,
    uint8_t value
)
{
    constexpr uint8_t BLOCK_COUNT = 10;
    constexpr int16_t BLOCK_WIDTH  = 18;
    constexpr int16_t BLOCK_HEIGHT = 12;
    constexpr int16_t BLOCK_GAP    = 3;

    uint8_t filledBlocks = value / 10;

    if (filledBlocks < 1)
    {
        filledBlocks = 1;
    }

    if (filledBlocks > BLOCK_COUNT)
    {
        filledBlocks = BLOCK_COUNT;
    }

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
            // Leeg blok
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

void BrightnessScreen::update()
{
}

void BrightnessScreen::onKeyEvent(
    const TCA8418::KeyEvent& event
)
{
    if (event.type != TCA8418::EventType::DOWN)
    {
        return;
    }

    // Terug naar menu
    if (event.key == TCA8418::Key::ESC ||
        event.key == TCA8418::Key::SKL)
    {
        if (backCallback != nullptr)
        {
            backCallback();
        }

        return;
    }

    // Selectie: Handheld / Display(s)
    if (event.key == TCA8418::Key::UP)
    {
        if (selectedItem > 0)
        {
            selectedItem--;
            draw();
        }

        return;
    }

    if (event.key == TCA8418::Key::DOWN)
    {
        if (selectedItem < 1)
        {
            selectedItem++;
            draw();
        }

        return;
    }

    // Helderheid lager
    if (event.key == TCA8418::Key::LEFT)
    {
        uint8_t* value = nullptr;

        if (selectedItem == 0)
        {
            value = &handheldBrightness;
        }
        else
        {
            value = &displayBrightness;
        }

        if (*value > 10)
        {
            *value -= 10;

            if (selectedItem == 0)
            {
                TFT.setBacklight(handheldBrightness);
            }
            else if (rfManager != nullptr)
            {
                rfManager->sendBrightness(displayBrightness);
            }

            draw();
        }

        return;
    }

    // Helderheid hoger
    if (event.key == TCA8418::Key::RIGHT)
    {
        uint8_t* value = nullptr;

        if (selectedItem == 0)
        {
            value = &handheldBrightness;
        }
        else
        {
            value = &displayBrightness;
        }

        if (*value < 100)
        {
            *value += 10;

            if (selectedItem == 0)
            {
                TFT.setBacklight(handheldBrightness);
            }
            else if (rfManager != nullptr)
            {
                rfManager->sendBrightness(displayBrightness);
            }

            draw();
        }

        return;
    }

    // OK bevestigen
    if (event.key == TCA8418::Key::OK)
    {
        // Voorlopig alleen bevestigen.
        // Hardware/RS485-aansturing volgt later.
        draw();
        return;
    }
}