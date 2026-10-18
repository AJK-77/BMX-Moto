#include "Displays/MiniDisplay/miniTFT.h"
#include "Displays/MiniDisplay/RapidWheelsLogo.h"

MiniTFTClass MiniTFT;

bool MiniTFTClass::begin()
{
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);

    tft.init();
    tft.setRotation(3);
    tft.invertDisplay(true);

    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(2);
    tft.setTextSize(1);

    return true;
}

void MiniTFTClass::showSplash()
{
    tft.fillScreen(TFT_BLACK);

    tft.pushImage(
        60,
        25,
        RAPID_WHEELS_LOGO_WIDTH,
        RAPID_WHEELS_LOGO_HEIGHT,
        RAPID_WHEELS_LOGO
    );

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(2);
    tft.setTextSize(1);

    tft.drawString("BMX Moto Counter", 80, 120);
    tft.drawString(
        "Made By A.J. Klarenbeek For Rapid Wheels",
        10,
        145
    );
    tft.drawString(
        "Contact: ajklarenbeek@gmail.com",
        40,
        165
    );

    splashStart = millis();
}

void MiniTFTClass::clear()
{
    tft.fillScreen(TFT_BLACK);
}

bool MiniTFTClass::splashFinished()
{
    return (millis() - splashStart) >= 5000;
}

void MiniTFTClass::drawText(
    int16_t x,
    int16_t y,
    const char* text,
    uint8_t size
)
{
    tft.setTextSize(size);
    tft.drawString(text, x, y);
}

