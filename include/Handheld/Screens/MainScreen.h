#pragma once

#include "Handheld/Input/TCA8418.h"
#include "Common/RaceState.h"

class MainScreen
{
public:
    MainScreen();

    void begin();
    void draw();
    void update();

    void onKeyEvent(const TCA8418::KeyEvent& event);

    void setRaceState(RaceState* state);

private:
    RaceState* raceState = nullptr;
};