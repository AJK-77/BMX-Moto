#pragma once

#include "Handheld/Input/TCA8418.h"

class FirmwareScreen
{
public:
    using BackCallback = void (*)();

    FirmwareScreen();

    void begin();
    void draw();
    void update();

    void setBackCallback(BackCallback callback);
    void onKeyEvent(const TCA8418::KeyEvent& event);

private:
    BackCallback backCallback = nullptr;
};