#pragma once

#include "Handheld/Input/TCA8418.h"
#include "Common/RaceState.h"

class MainScreen
{
public:
    using StateChangeCallback = void (*)(void);
    using MenuCallback = void (*)();
    using SelectCallback = void (*)();

    MainScreen();

    void begin();
    void draw();
    void update();

    void setRaceState(RaceState* state);
    void setStateChangeCallback(StateChangeCallback callback);
    void setMenuCallback(MenuCallback callback);

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

    MenuCallback menuCallback = nullptr;

    bool inputMode = false;
    uint16_t inputValue = 0;
    uint8_t inputDigits = 0;
    char tempRaceNumber[5] = "0000";
    void drawLabel(EditField field);

    void startInput();
    void cancelInput();
    void confirmInput();
    void inputDigit(uint8_t digit);
    
};