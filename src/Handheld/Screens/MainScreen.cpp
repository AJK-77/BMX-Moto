#include <Arduino.h>

#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"
#include "Handheld/Screens/MainScreen.h"


MainScreen::MainScreen()
{
}


void MainScreen::begin()
{
    selectedField = EditField::MOTO;
    inputMode = false;
    inputValue = 0;
    inputDigits = 0;
}


void MainScreen::setRaceState(RaceState* state)
{
    raceState = state;

    if (raceState != nullptr &&
        raceState->getMode() == RaceMode::AUTO)
    {
        selectedField = EditField::MOTO;
    }
}


void MainScreen::setStateChangeCallback(StateChangeCallback callback)
{
    stateChangeCallback = callback;
}


void MainScreen::draw()
{
    TFT.clear();

    // --------------------------------------------------------
    // Status
    // --------------------------------------------------------

    TFT.drawText(
        2,
        GuiLayout::STATUS_Y + 5,
        "RF OK",
        1
    );


    // --------------------------------------------------------
    // Labels
    // --------------------------------------------------------

    if (raceState != nullptr &&
        raceState->getMode() == RaceMode::AUTO)
    {
        // In AUTO geen selectie-indicatie.
        TFT.drawText(
            25,
            GuiLayout::RACE_TOP,
            "MANCHE",
            2
        );

        TFT.drawText(
            215,
            GuiLayout::RACE_TOP,
            "MOTO",
            2
        );
    }
    else if (selectedField == EditField::MANCHE)
    {
        TFT.drawTextInverted(
            25,
            GuiLayout::RACE_TOP,
            "MANCHE",
            2
        );

        TFT.drawText(
            215,
            GuiLayout::RACE_TOP,
            "MOTO",
            2
        );
    }
    else
    {
        TFT.drawText(
            25,
            GuiLayout::RACE_TOP,
            "MANCHE",
            2
        );

        TFT.drawTextInverted(
            215,
            GuiLayout::RACE_TOP,
            "MOTO",
            2
        );
    }


    // --------------------------------------------------------
    // Race number
    // --------------------------------------------------------

    const char* raceNumber = "0000";

    if (raceState != nullptr)
    {
        raceNumber = raceState->getRaceNumber();
    }

    // RaceState:
    //
    // [0] = Manche
    // [1] = Moto honderden
    // [2] = Moto tientallen
    // [3] = Moto eenheden
    //
    // RaceState is altijd exact vier tekens.


    // --------------------------------------------------------
    // Tijdens directe invoer:
    //
    // RaceState blijft ongewijzigd.
    // Alleen de weergave op de HH verandert.
    //
    // Zolang er nog geen cijfer is ingevoerd:
    //   MANCHE -> "-"
    //   MOTO   -> "---"
    //
    // Zodra het eerste cijfer wordt ingevoerd,
    // wordt de tijdelijke invoer weergegeven.
    // --------------------------------------------------------

    char mancheText[5];
    char motoText[4];

    if (inputMode &&
        inputDigits == 0 &&
        selectedField == EditField::MANCHE)
    {
        snprintf(
            mancheText,
            sizeof(mancheText),
            "-"
        );
    }
    else if (inputMode &&
             selectedField == EditField::MANCHE)
    {
        snprintf(
            mancheText,
            sizeof(mancheText),
            "%u",
            inputValue
        );
    }
    else
    {
        mancheText[0] = raceNumber[0];
        mancheText[1] = '\0';
    }


    if (inputMode &&
        inputDigits == 0 &&
        selectedField == EditField::MOTO)
    {
        snprintf(
            motoText,
            sizeof(motoText),
            "---"
        );
    }
    else if (inputMode &&
             selectedField == EditField::MOTO)
    {
        snprintf(
            motoText,
            sizeof(motoText),
            "%03u",
            inputValue
        );
    }
    else
    {
        motoText[0] = raceNumber[1];
        motoText[1] = raceNumber[2];
        motoText[2] = raceNumber[3];
        motoText[3] = '\0';
    }


    // --------------------------------------------------------
    // Grote waarden
    // --------------------------------------------------------

    TFT.drawText(
        55,
        60,
        mancheText,
        6
    );

    TFT.drawText(
        170,
        60,
        motoText,
        6
    );


    // --------------------------------------------------------
    // Softkeys
    // --------------------------------------------------------

    TFT.drawText(
        20,
        166,
        "MENU",
        2
    );

    if (raceState != nullptr &&
        raceState->getMode() == RaceMode::AUTO)
    {
        TFT.drawText(
            240,
            166,
            "AUTO",
            2
        );
    }
    else
    {
        TFT.drawText(
            240,
            166,
            "MAN",
            2
        );
    }


    // --------------------------------------------------------
    // Battery
    // --------------------------------------------------------

    TFT.drawBattery(
        true,
        true,
        true,
        true
    );
}


void MainScreen::update()
{
}


// ============================================================
// Key handling
// ============================================================

void MainScreen::onKeyEvent(const TCA8418::KeyEvent& event)
{
    // Alleen key-down verwerken.
    if (event.type != TCA8418::EventType::DOWN)
    {
        return;
    }

    if (raceState == nullptr)
    {
        return;
    }


    // ========================================================
    // Direct input mode
    // ========================================================

    if (inputMode)
    {
        // ----------------------------------------------------
        // C of ESC = cancel
        // ----------------------------------------------------

        if (event.key == TCA8418::Key::C ||
            event.key == TCA8418::Key::ESC)
        {
            cancelInput();
            return;
        }


        // ----------------------------------------------------
        // OK = confirm
        // ----------------------------------------------------

        if (event.key == TCA8418::Key::OK)
        {
            confirmInput();
            return;
        }


        // ----------------------------------------------------
        // Numeric input
        // ----------------------------------------------------

        switch (event.key)
        {
            case TCA8418::Key::NUM_0:
                inputDigit(0);
                return;

            case TCA8418::Key::NUM_1:
                inputDigit(1);
                return;

            case TCA8418::Key::NUM_2:
                inputDigit(2);
                return;

            case TCA8418::Key::NUM_3:
                inputDigit(3);
                return;

            case TCA8418::Key::NUM_4:
                inputDigit(4);
                return;

            case TCA8418::Key::NUM_5:
                inputDigit(5);
                return;

            case TCA8418::Key::NUM_6:
                inputDigit(6);
                return;

            case TCA8418::Key::NUM_7:
                inputDigit(7);
                return;

            case TCA8418::Key::NUM_8:
                inputDigit(8);
                return;

            case TCA8418::Key::NUM_9:
                inputDigit(9);
                return;

            default:
                break;
        }

        return;
    }


    // ========================================================
    // Normal mode
    // ========================================================


    // --------------------------------------------------------
    // LEFT = select MANCHE
    // --------------------------------------------------------

    if (event.key == TCA8418::Key::LEFT)
    {
        if (raceState->getMode() == RaceMode::MANUAL)
        {
            selectedField = EditField::MANCHE;
            draw();
        }

        return;
    }


    // --------------------------------------------------------
    // RIGHT = select MOTO
    // --------------------------------------------------------

    if (event.key == TCA8418::Key::RIGHT)
    {
        selectedField = EditField::MOTO;
        draw();

        return;
    }


    // --------------------------------------------------------
    // OK = direct input
    // --------------------------------------------------------

    if (event.key == TCA8418::Key::OK)
    {
        startInput();
        return;
    }


    // ========================================================
    // +
    // Werkt in MANUAL en AUTO
    // ========================================================

    if (event.key == TCA8418::Key::PLUS)
    {
        char newRaceNumber[5];

        memcpy(
            newRaceNumber,
            raceState->getRaceNumber(),
            sizeof(newRaceNumber)
        );

        if (selectedField == EditField::MANCHE)
        {
            // Manche alleen handmatig wijzigen.
            if (raceState->getMode() != RaceMode::MANUAL)
            {
                return;
            }

            uint8_t manche = 0;

            if (newRaceNumber[0] >= '0' &&
                newRaceNumber[0] <= '9')
            {
                manche = newRaceNumber[0] - '0';
            }

            manche++;

            if (manche > 9)
            {
                manche = 0;
            }

            newRaceNumber[0] = '0' + manche;
        }
        else
        {
            uint16_t moto = 0;

            if (newRaceNumber[1] >= '0' &&
                newRaceNumber[1] <= '9')
            {
                moto +=
                    (newRaceNumber[1] - '0') * 100;
            }

            if (newRaceNumber[2] >= '0' &&
                newRaceNumber[2] <= '9')
            {
                moto +=
                    (newRaceNumber[2] - '0') * 10;
            }

            if (newRaceNumber[3] >= '0' &&
                newRaceNumber[3] <= '9')
            {
                moto +=
                    newRaceNumber[3] - '0';
            }

            moto++;

            if (moto > 999)
            {
                moto = 0;
            }

            newRaceNumber[1] = '0' + (moto / 100);
            newRaceNumber[2] = '0' + ((moto / 10) % 10);
            newRaceNumber[3] = '0' + (moto % 10);
        }

        raceState->setRaceNumber(newRaceNumber);
        raceState->nextEvent();

        draw();

        if (stateChangeCallback != nullptr)
        {
            stateChangeCallback();
        }

        return;
    }


    // ========================================================
    // -
    // Werkt in MANUAL en AUTO
    // ========================================================

    if (event.key == TCA8418::Key::MINUS)
    {
        char newRaceNumber[5];

        memcpy(
            newRaceNumber,
            raceState->getRaceNumber(),
            sizeof(newRaceNumber)
        );

        if (selectedField == EditField::MANCHE)
        {
            // Manche alleen handmatig wijzigen.
            if (raceState->getMode() != RaceMode::MANUAL)
            {
                return;
            }

            uint8_t manche = 0;

            if (newRaceNumber[0] >= '0' &&
                newRaceNumber[0] <= '9')
            {
                manche = newRaceNumber[0] - '0';
            }

            if (manche == 0)
            {
                manche = 9;
            }
            else
            {
                manche--;
            }

            newRaceNumber[0] = '0' + manche;
        }
        else
        {
            uint16_t moto = 0;

            if (newRaceNumber[1] >= '0' &&
                newRaceNumber[1] <= '9')
            {
                moto +=
                    (newRaceNumber[1] - '0') * 100;
            }

            if (newRaceNumber[2] >= '0' &&
                newRaceNumber[2] <= '9')
            {
                moto +=
                    (newRaceNumber[2] - '0') * 10;
            }

            if (newRaceNumber[3] >= '0' &&
                newRaceNumber[3] <= '9')
            {
                moto +=
                    newRaceNumber[3] - '0';
            }

            if (moto == 0)
            {
                moto = 999;
            }
            else
            {
                moto--;
            }

            newRaceNumber[1] = '0' + (moto / 100);
            newRaceNumber[2] = '0' + ((moto / 10) % 10);
            newRaceNumber[3] = '0' + (moto % 10);
        }

        raceState->setRaceNumber(newRaceNumber);
        raceState->nextEvent();

        draw();

        if (stateChangeCallback != nullptr)
        {
            stateChangeCallback();
        }

        return;
    }


    // ========================================================
    // SKR
    // MANUAL <-> AUTO
    // ========================================================

    if (event.key == TCA8418::Key::SKR)
    {
        if (raceState->getMode() == RaceMode::AUTO)
        {
            raceState->setMode(RaceMode::MANUAL);
        }
        else
        {
            raceState->setMode(RaceMode::AUTO);

            // In AUTO is alleen MOTO selecteerbaar.
            selectedField = EditField::MOTO;
        }

        raceState->nextEvent();

        draw();

        if (stateChangeCallback != nullptr)
        {
            stateChangeCallback();
        }

        return;
    }
}


// ============================================================
// Direct input
// ============================================================

void MainScreen::startInput()
{
    if (raceState == nullptr)
    {
        return;
    }

    // In AUTO mag alleen MOTO worden ingevoerd.
    if (raceState->getMode() == RaceMode::AUTO)
    {
        selectedField = EditField::MOTO;
    }

    inputMode = true;
    inputValue = 0;
    inputDigits = 0;

    draw();
}


void MainScreen::cancelInput()
{
    inputMode = false;
    inputValue = 0;
    inputDigits = 0;

    // RaceState is nooit aangepast tijdens invoer,
    // dus draw() toont automatisch de oude waarde weer.
    draw();
}


void MainScreen::confirmInput()
{
    if (!inputMode || raceState == nullptr)
    {
        return;
    }

    // Geen cijfers ingevoerd = niets wijzigen.
    if (inputDigits == 0)
    {
        cancelInput();
        return;
    }


    char newRaceNumber[5];

    memcpy(
        newRaceNumber,
        raceState->getRaceNumber(),
        sizeof(newRaceNumber)
    );


    if (selectedField == EditField::MANCHE)
    {
        // Manche kan alleen in MANUAL worden gewijzigd.
        if (raceState->getMode() != RaceMode::MANUAL)
        {
            cancelInput();
            return;
        }

        if (inputValue > 9)
        {
            inputValue = 9;
        }

        newRaceNumber[0] = '0' + inputValue;
    }
    else
    {
        if (inputValue > 999)
        {
            inputValue = 999;
        }

        newRaceNumber[1] = '0' + (inputValue / 100);
        newRaceNumber[2] = '0' + ((inputValue / 10) % 10);
        newRaceNumber[3] = '0' + (inputValue % 10);
    }


    // --------------------------------------------------------
    // Pas hier wordt de waarde daadwerkelijk opgeslagen.
    // --------------------------------------------------------

    raceState->setRaceNumber(newRaceNumber);
    raceState->nextEvent();

    inputMode = false;
    inputValue = 0;
    inputDigits = 0;

    draw();

    if (stateChangeCallback != nullptr)
    {
        stateChangeCallback();
    }
}


void MainScreen::inputDigit(uint8_t digit)
{
    if (!inputMode || raceState == nullptr)
    {
        return;
    }

    uint8_t maxDigits = 3;

    if (selectedField == EditField::MANCHE)
    {
        maxDigits = 1;
    }


    if (inputDigits >= maxDigits)
    {
        return;
    }


    uint16_t newValue =
        (inputValue * 10) + digit;


    // --------------------------------------------------------
    // Waarde buiten bereik?
    // --------------------------------------------------------

    if (selectedField == EditField::MANCHE &&
        newValue > 9)
    {
        return;
    }

    if (selectedField == EditField::MOTO &&
        newValue > 999)
    {
        return;
    }


    inputValue = newValue;
    inputDigits++;

    draw();
}