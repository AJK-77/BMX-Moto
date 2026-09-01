#pragma once

#include "Handheld/Input/TCA8418.h"

class MenuScreen
{
public:
    MenuScreen();

    void begin();
    void draw();
    void update();

    void onKeyEvent(const TCA8418::KeyEvent& event);
};

extern MenuScreen menuScreen;