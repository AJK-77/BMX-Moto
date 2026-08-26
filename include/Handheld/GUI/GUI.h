#pragma once

#include <Arduino.h>

class GUI
{
public:
    GUI();

    void begin();
    void update();

private:
    bool initialized = false;
};