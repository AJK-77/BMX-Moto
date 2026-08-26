#pragma once

#include <Arduino.h>

namespace GuiLayout
{
    // ============================================================
    // Fysiek TFT
    // ============================================================

    constexpr int16_t SCREEN_WIDTH  = 320;
    constexpr int16_t SCREEN_HEIGHT = 240;

    // ============================================================
    // GUI zichtbaar gebied
    //
    // De eerste 20 fysieke pixels blijven vrij.
    // GUI Y=0 begint dus op fysieke Y=20.
    // ============================================================

    constexpr int16_t VIEW_X = 0;
    constexpr int16_t VIEW_Y = 20;

    constexpr int16_t VIEW_WIDTH  = 320;
    constexpr int16_t VIEW_HEIGHT = 200;

    // ============================================================
    // Tekstmarge
    // ============================================================

    constexpr int16_t TEXT_MARGIN_LEFT = 2;

    // ============================================================
    // Statusbalk
    // ============================================================

    constexpr int16_t STATUS_Y      = 0;
    constexpr int16_t STATUS_HEIGHT = 24;

    // ============================================================
    // Racegedeelte
    // ============================================================

    constexpr int16_t RACE_TOP    = 30;
    constexpr int16_t RACE_BOTTOM = 160;

    constexpr int16_t RACE_CENTER_X = 160;

    constexpr int16_t MANCHE_CENTER_X = 80;
    constexpr int16_t MOTO_CENTER_X  = 240;

    // ============================================================
    // Softkey gedeelte
    // ============================================================

    constexpr int16_t SOFTKEY_Y = 166;

    constexpr int16_t SKL_X = 40;
    constexpr int16_t SKR_X = 280;

    // ============================================================
    // Coördinaten omrekenen
    // ============================================================

    constexpr int16_t toTftX(int16_t x)
    {
        return VIEW_X + x;
    }

    constexpr int16_t toTftY(int16_t y)
    {
        return VIEW_Y + y;
    }

    constexpr int16_t textX(int16_t x)
    {
        return VIEW_X + TEXT_MARGIN_LEFT + x;
    }
}