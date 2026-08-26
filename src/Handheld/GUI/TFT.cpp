#include "Handheld/GUI/TFT.h"
#include "Handheld/Pins.h"
#include "Handheld/GUI/GuiLayout.h"
#include "Handheld/GUI/Assets/RapidWheelsLogo.h"


// Display layout.
// Het fysieke TFT is 320x240.
// De bovenste en onderste 20 pixels vallen buiten het zichtbare venster.

constexpr int DISPLAY_OFFSET_TOP    = 20;
constexpr int DISPLAY_OFFSET_BOTTOM = 20;


TFTClass TFT;


bool TFTClass::begin()
{
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);

    tft.init();
    tft.setRotation(1);
    tft.invertDisplay(true);

    clear();

    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    return true;
}


void TFTClass::clear()
{
    tft.fillScreen(TFT_BLACK);
}


void TFTClass::update()
{
    // Voor later
}


void TFTClass::drawText(
    int16_t x,
    int16_t y,
    const char *text,
    uint8_t size)
{
    tft.setTextSize(size);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.setCursor(
        GuiLayout::textX(x),
        GuiLayout::toTftY(y)
    );

    tft.print(text);
}

void TFTClass::showSplash()
{
    clear();

    drawLogo();

    drawText(0, 105, "BMX Handheld");
    drawText(0, 126, "Initialising...");

    splashStart = millis();
}

bool TFTClass::splashFinished()
{
    return (millis() - splashStart) >= 3000;
}


void TFTClass::showKey(const char *text)
{
    clear();

    drawText(10, 2, "Key:");

    tft.setTextFont(4);
    drawText(10, 32, text);
    tft.setTextFont(2);
}


void TFTClass::showLine(int y)
{
    clear();

    // Lijn
    tft.drawFastHLine(
        0,
        y + DISPLAY_OFFSET_TOP,
        320,
        TFT_WHITE
    );

    // Y-waarde in het midden
    char buffer[16];
    sprintf(buffer, "Y = %d", y);

    tft.setTextDatum(MC_DATUM);
    tft.drawString(buffer, 160, 120);

    // Terug naar standaard
    tft.setTextDatum(TL_DATUM);
}

void TFTClass::drawLogo()
{
    constexpr int16_t x =
        (GuiLayout::VIEW_WIDTH - RAPID_WHEELS_LOGO_WIDTH) / 2;

    constexpr int16_t y = 5;

    for (int16_t py = 0; py < RAPID_WHEELS_LOGO_HEIGHT; py++)
    {
        for (int16_t px = 0; px < RAPID_WHEELS_LOGO_WIDTH; px++)
        {
            const uint16_t index =
                py * RAPID_WHEELS_LOGO_WIDTH + px;

            if (pgm_read_byte(
                    &RAPID_WHEELS_LOGO_MASK[index >> 3]
                ) & (0x80 >> (index & 7)))
            {
                const uint16_t color =
                    pgm_read_word(&RAPID_WHEELS_LOGO[index]);

                tft.drawPixel(
                    GuiLayout::toTftX(x + px),
                    GuiLayout::toTftY(y + py),
                    color
                );
            }
        }
    }
}