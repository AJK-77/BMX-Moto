#include <Arduino.h>

#include "Handheld/GUI/TFT.h"
#include "Handheld/GUI/GuiLayout.h"
#include "Handheld/Screens/MainScreen.h"
#include <string.h>


MainScreen::MainScreen()
{
}


void MainScreen::begin()
{
    selectedField = EditField::RACE;
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
        selectedField = EditField::RACE;
    }
}


void MainScreen::setStateChangeCallback(StateChangeCallback callback)
{
    stateChangeCallback = callback;
}

void MainScreen::setMenuCallback(MenuCallback callback)
{
    menuCallback = callback;
}

void MainScreen::onHeartbeatLost(uint8_t node)
{
    heartbeatErrorNode = node;
    draw();
}

void MainScreen::onHeartbeatRestored()
{
    heartbeatErrorNode = 0;
    draw();
}

void MainScreen::draw()
{
    //TFT.clearFull();
    TFT.clear();

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
            "MOTO",
            2
        );

        TFT.drawText(
            215,
            GuiLayout::RACE_TOP,
            "RACE",
            2
        );
    }
    else if (selectedField == EditField::MANCHE)
    {
        TFT.drawTextInverted(
            25,
            GuiLayout::RACE_TOP,
            "MOTO",
            2
        );

        TFT.drawText(
            215,
            GuiLayout::RACE_TOP,
            "RACE",
            2
        );
    }
    else
    {
        TFT.drawText(
            25,
            GuiLayout::RACE_TOP,
            "MOTO",
            2
        );

        TFT.drawTextInverted(
            215,
            GuiLayout::RACE_TOP,
            "RACE",
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
    // [1] = RACE honderden
    // [2] = RACE tientallen
    // [3] = RACE eenheden
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
    //   RACE   -> "---"
    //
    // Zodra het eerste cijfer wordt ingevoerd,
    // wordt de tijdelijke invoer weergegeven.
    // --------------------------------------------------------

    char mancheText[5];
    char raceText[4];

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
else if (inputMode)
{
    mancheText[0] = tempRaceNumber[0];
    mancheText[1] = '\0';
}
else
{
    mancheText[0] = raceNumber[0];
    mancheText[1] = '\0';
}


    if (inputMode &&
    inputDigits == 0 &&
    selectedField == EditField::RACE)
{
    snprintf(
        raceText,
        sizeof(raceText),
        "---"
    );
}
else if (inputMode &&
         selectedField == EditField::RACE)
{
    snprintf(
        raceText,
        sizeof(raceText),
        "%03u",
        inputValue
    );
}
else if (inputMode)
{
    raceText[0] = tempRaceNumber[1];
    raceText[1] = tempRaceNumber[2];
    raceText[2] = tempRaceNumber[3];
    raceText[3] = '\0';
}
else
{
    raceText[0] = raceNumber[1];
    raceText[1] = raceNumber[2];
    raceText[2] = raceNumber[3];
    raceText[3] = '\0';
}


    // --------------------------------------------------------
    // Grote waarden
    // --------------------------------------------------------

    TFT.drawText(
        30,
        60,
        mancheText,
        8
    );

    TFT.drawText(
        140,
        60,
        raceText,
        8
    );

    // --------------------------------------------------------
    // Bevestiging invoer
    // --------------------------------------------------------

    if (inputMode)
    {
     TFT.drawText(
            70,
            145,
            "DRUK OK OM TE BEVESTIGEN!",
            1
    );
}


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
        TFT.drawTextInverted(
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

    if (event.key == TCA8418::Key::SKL)
{
    if (menuCallback != nullptr)
    {
        menuCallback();
    }

    return;
}

    // ========================================================
    // Direct input mode
    // ========================================================

    if (inputMode)
    {
        // ----------------------------------------------------
        // LEFT / RIGHT = wissel tussen MANCHE en RACE
        // ----------------------------------------------------

        if (event.key == TCA8418::Key::LEFT ||
    event.key == TCA8418::Key::RIGHT)
{
    // Eerst de huidige invoer opslaan
    if (inputDigits > 0)
    {
        if (selectedField == EditField::MANCHE)
        {
            tempRaceNumber[0] = '0' + inputValue;
        }
        else
        {
            tempRaceNumber[1] = '0' + (inputValue / 100);
            tempRaceNumber[2] = '0' + ((inputValue / 10) % 10);
            tempRaceNumber[3] = '0' + (inputValue % 10);
        }
    }

    // In AUTO mag alleen Race worden bewerkt
    if (raceState->getMode() == RaceMode::AUTO)
    {
        selectedField = EditField::RACE;
    }
    else
    {
        if (event.key == TCA8418::Key::LEFT)
            selectedField = EditField::MANCHE;
        else
            selectedField = EditField::RACE;
    }

    // Nieuwe invoer voor het nieuwe veld
    inputValue = 0;
    inputDigits = 0;

    draw();
    return;
}

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
    // RIGHT = select RACE
    // --------------------------------------------------------

    if (event.key == TCA8418::Key::RIGHT)
    {
        selectedField = EditField::RACE;
        draw();

        return;
    }

// ========================================================
// Numeric input
// Eerste cijfer start direct de invoer
// ========================================================

switch (event.key)
{
    case TCA8418::Key::NUM_0:
        startInput();
        inputDigit(0);
        return;

    case TCA8418::Key::NUM_1:
        startInput();
        inputDigit(1);
        return;

    case TCA8418::Key::NUM_2:
        startInput();
        inputDigit(2);
        return;

    case TCA8418::Key::NUM_3:
        startInput();
        inputDigit(3);
        return;

    case TCA8418::Key::NUM_4:
        startInput();
        inputDigit(4);
        return;

    case TCA8418::Key::NUM_5:
        startInput();
        inputDigit(5);
        return;

    case TCA8418::Key::NUM_6:
        startInput();
        inputDigit(6);
        return;

    case TCA8418::Key::NUM_7:
        startInput();
        inputDigit(7);
        return;

    case TCA8418::Key::NUM_8:
        startInput();
        inputDigit(8);
        return;

    case TCA8418::Key::NUM_9:
        startInput();
        inputDigit(9);
        return;

    default:
        break;
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
            uint16_t race = 0;

            if (newRaceNumber[1] >= '0' &&
                newRaceNumber[1] <= '9')
            {
                race +=
                    (newRaceNumber[1] - '0') * 100;
            }

            if (newRaceNumber[2] >= '0' &&
                newRaceNumber[2] <= '9')
            {
                race +=
                    (newRaceNumber[2] - '0') * 10;
            }

            if (newRaceNumber[3] >= '0' &&
                newRaceNumber[3] <= '9')
            {
                race +=
                    newRaceNumber[3] - '0';
            }

            race++;

            if (race > 999)
            {
                race = 0;
            }

            newRaceNumber[1] = '0' + (race / 100);
            newRaceNumber[2] = '0' + ((race / 10) % 10);
            newRaceNumber[3] = '0' + (race % 10);
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
            uint16_t race = 0;

            if (newRaceNumber[1] >= '0' &&
                newRaceNumber[1] <= '9')
            {
                race +=
                    (newRaceNumber[1] - '0') * 100;
            }

            if (newRaceNumber[2] >= '0' &&
                newRaceNumber[2] <= '9')
            {
                race +=
                    (newRaceNumber[2] - '0') * 10;
            }

            if (newRaceNumber[3] >= '0' &&
                newRaceNumber[3] <= '9')
            {
                race +=
                    newRaceNumber[3] - '0';
            }

            if (race == 0)
            {
                race = 999;
            }
            else
            {
                race--;
            }

            newRaceNumber[1] = '0' + (race / 100);
            newRaceNumber[2] = '0' + ((race / 10) % 10);
            newRaceNumber[3] = '0' + (race % 10);
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

            // In AUTO is alleen RACE selecteerbaar.
            selectedField = EditField::RACE;
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

    // In AUTO mag alleen RACE worden ingevoerd.
    if (raceState->getMode() == RaceMode::AUTO)
    {
        selectedField = EditField::RACE;
    }

    // Maak een tijdelijke kopie van de huidige RaceState.
    memcpy(
        tempRaceNumber,
        raceState->getRaceNumber(),
        sizeof(tempRaceNumber)
    );

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
    if (raceState == nullptr)
    {
        cancelInput();
        return;
    }

    // Laatste veld eerst opslaan
    if (inputDigits > 0)
    {
        if (selectedField == EditField::MANCHE)
        {
            tempRaceNumber[0] = '0' + inputValue;
        }
        else
        {
            tempRaceNumber[1] = '0' + (inputValue / 100);
            tempRaceNumber[2] = '0' + ((inputValue / 10) % 10);
            tempRaceNumber[3] = '0' + (inputValue % 10);
        }
    }

    // Controleren of er daadwerkelijk iets veranderd is
    if (memcmp(
            tempRaceNumber,
            raceState->getRaceNumber(),
            4) == 0)
    {
        cancelInput();
        return;
    }

    // Alles in één keer naar RaceState
    raceState->setRaceNumber(tempRaceNumber);
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

    if (selectedField == EditField::RACE &&
        newValue > 999)
    {
        return;
    }


    inputValue = newValue;
    inputDigits++;

    draw();
}