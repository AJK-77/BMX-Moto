#include "Handheld/Screens/MainScreen.h"

#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"


MainScreen::MainScreen()
{
}


void MainScreen::begin()
{
}


void MainScreen::draw()
{
    TFT.clear();

    // ============================================================
    // Statusbalk
    // ============================================================

    TFT.drawText(2, GuiLayout::STATUS_Y + 5, "RF OK", 1);
    

    /// ============================================================
    // Race labels
    /// ============================================================

    TFT.drawText(25, GuiLayout::RACE_TOP, "MANCHE", 2);
    TFT.drawText(215, GuiLayout::RACE_TOP, "MOTO", 2);


    // ============================================================
    // Race waarden
    // ============================================================

    TFT.drawText(55, 60, "0", 6);
    TFT.drawText(170, 60, "000", 6);


    // ============================================================
    // Softkeys
    // ============================================================

    TFT.drawText(20, 166, "MENU", 2);
    TFT.drawText(240, 166, "MODE", 2);

    TFT.drawBattery(true, true, true, true);
}


void MainScreen::update()
{
}