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
    RaceState* raceState = nullptr;
    StateChangeCallback stateChangeCallback = nullptr;
};