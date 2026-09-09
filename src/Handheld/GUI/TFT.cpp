#include "Handheld/GUI/TFT.h"
#include "Handheld/Pins.h"
#include "Handheld/GUI/GuiLayout.h"
#include "Handheld/GUI/Assets/RapidWheelsLogo.h"

// Display layout.
// Het fysieke TFT is 320x240.
// De bovenste en onderste 20 pixels vallen buiten het zichtbare venster.

constexpr int DISPLAY_OFFSET_TOP    = 20;
constexpr int DISPLAY_OFFSET_BOTTOM = 20;

constexpr uint8_t BACKLIGHT_CHANNEL = 0;
constexpr uint32_t BACKLIGHT_FREQUENCY = 5000;
constexpr uint8_t BACKLIGHT_RESOLUTION = 8;


TFTClass TFT;


bool TFTClass::begin()
{
    tft.init();
    tft.setRotation(1);
    tft.invertDisplay(true);

    clear();

    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    pinMode(TFT_BACKLIGHT, OUTPUT);

    ledcSetup(
        BACKLIGHT_CHANNEL,
        BACKLIGHT_FREQUENCY,
        BACKLIGHT_RESOLUTION
    );

    ledcAttachPin(
        TFT_BACKLIGHT,
        BACKLIGHT_CHANNEL
    );

    setBacklight(100);

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

void TFTClass::drawTextInverted(
    int16_t x,
    int16_t y,
    const char *text,
    uint8_t size)
{
    tft.setTextSize(size);

    int16_t textX = GuiLayout::textX(x);
    int16_t textY = GuiLayout::toTftY(y);

    int16_t textWidth = tft.textWidth(text);
    int16_t textHeight = tft.fontHeight();

    constexpr int16_t paddingX = 2;
    constexpr int16_t paddingY = -2;

    tft.fillRect(
        textX - paddingX,
        textY - paddingY,
        textWidth + (paddingX * 2),
        textHeight + (paddingY * 2),
        TFT_WHITE
    );

    // Alleen de letters tekenen.
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(textX, textY);
    tft.print(text);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void TFTClass::showSplash()
{
    clear();

    drawLogo();

    drawText(120, 125, "BMX Handheld");
    drawText(120, 146, "Initialising...");

    splashStart = millis();
}

bool TFTClass::splashFinished()
{
    return (millis() - splashStart) >= 5000;
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

void TFTClass::fillRect(
    int16_t x,
    int16_t y,
    int16_t width,
    int16_t height,
    uint16_t color)
{
    tft.fillRect(
        GuiLayout::toTftX(x),
        GuiLayout::toTftY(y),
        width,
        height,
        color
    );
}

void TFTClass::drawBattery(
    bool led25,
    bool led50,
    bool led75,
    bool led100)
{
    constexpr int16_t x = 278;
    constexpr int16_t y = GuiLayout::STATUS_Y + 3;

    // ============================================================
    // Batterij outline
    // ============================================================

    constexpr int16_t bodyWidth  = 38;
    constexpr int16_t bodyHeight = 16;

    tft.drawRect(
        GuiLayout::toTftX(x),
        GuiLayout::toTftY(y),
        bodyWidth,
        bodyHeight,
        TFT_WHITE
    );

    // ============================================================
    // Batterij contact
    // ============================================================

    tft.fillRect(
        GuiLayout::toTftX(x + bodyWidth),
        GuiLayout::toTftY(y + 5),
        3,
        6,
        TFT_WHITE
    );

    // ============================================================
    // Vier batterijsegmenten
    // ============================================================

    constexpr int16_t segmentWidth  = 5;
    constexpr int16_t segmentHeight = 8;

    constexpr int16_t segmentX   = 3;
    constexpr int16_t segmentY   = 4;
    constexpr int16_t segmentGap = 3;

    const bool leds[4] =
    {
        led25,
        led50,
        led75,
        led100
    };

    for (uint8_t i = 0; i < 4; i++)
    {
        if (!leds[i])
            continue;

        tft.fillRect(
            GuiLayout::toTftX(
                x + segmentX +
                (i * (segmentWidth + segmentGap))
            ),
            GuiLayout::toTftY(y + segmentY),
            segmentWidth,
            segmentHeight,
            TFT_WHITE
        );
    }
}

void TFTClass::setBacklight(uint8_t percent)
{
    if (percent < 10)
        percent = 10;

    if (percent > 100)
        percent = 100;

    const uint8_t duty =
        static_cast<uint8_t>(
            (static_cast<uint16_t>(percent) * 255) / 100
        );

    ledcWrite(
        BACKLIGHT_CHANNEL,
        duty
    );
}