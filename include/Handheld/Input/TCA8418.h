#pragma once

#include <Arduino.h>

class TCA8418
{
public:

    enum class EventType
    {
        DOWN,
        UP
    };

    enum class Key
    {
        NONE,

        NUM_0,
        NUM_1,
        NUM_2,
        NUM_3,
        NUM_4,
        NUM_5,
        NUM_6,
        NUM_7,
        NUM_8,
        NUM_9,

        PLUS,
        MINUS,

        C,
        ESC,
        OK,

        UP,
        DOWN,
        LEFT,
        RIGHT,

        SKL,
        SKR,

        SECOND_F,

        POWER
    };

    struct KeyEvent
    {
        Key key;
        EventType type;
    };

    using EventCallback = void (*)(const KeyEvent& event);

    bool begin();
    void update();

    void setEventCallback(EventCallback callback);

    const char* keyName(Key key);

    bool battery25();
    bool battery50();
    bool battery75();
    bool battery100();

private:

    bool battery25State = false;
    bool battery50State = false;
    bool battery75State = false;
    bool battery100State = false;

    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegister(uint8_t reg, uint8_t &value);

    bool configure();
    void readKeyEvents();

    Key matrixToKey(uint8_t keyNumber);

    void readBatteryInputs();

    EventCallback eventCallback = nullptr;
    
};

extern TCA8418 tca8418;