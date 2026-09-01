#pragma once

#include <Arduino.h>

class TCA8418
{
public:
    bool begin();
    void update();

private:
    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegister(uint8_t reg, uint8_t &value);

    bool configure();
    void readKeyEvents();
};

extern TCA8418 tca8418;