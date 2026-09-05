#pragma once

#include "Handheld/Input/TCA8418.h"
#include "Common/RaceState.h"

class MainScreen
{
public:
    using StateChangeCallback = void (*)(void);

    MainScreen();

    void begin();
    void draw();
    void update();

    void setRaceState(RaceState* state);
    void setStateChangeCallback(StateChangeCallback callback);

    void onKeyEvent(const TCA8418::KeyEvent& event);

private:
    enum class EditField
    {
        MANCHE,
        MOTO
    };

    RaceState* raceState = nullptr;
    StateChangeCallback stateChangeCallback = nullptr;

    EditField selectedField = EditField::MOTO;

    bool inputMode = false;
    uint16_t inputValue = 0;
    uint8_t inputDigits = 0;

    void drawLabel(EditField field);

    void startInput();
    void cancelInput();
    void confirmInput();
    void inputDigit(uint8_t digit);
};