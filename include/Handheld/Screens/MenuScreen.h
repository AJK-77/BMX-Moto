#pragma once

#include "Handheld/Input/TCA8418.h"

class MenuScreen
{
public:
    using BackCallback = void (*)();
    using SelectCallback = void (*)(uint8_t item);

    MenuScreen();

    void begin();
    void draw();
    void update();

    void setBackCallback(BackCallback callback);
    void setSelectCallback(SelectCallback callback);

    void onKeyEvent(const TCA8418::KeyEvent& event);
    

private:
    uint8_t selectedItem = 0;

    static constexpr uint8_t MENU_ITEM_COUNT = 4;

    void drawMenuItem(uint8_t index);

    BackCallback backCallback = nullptr;
    SelectCallback selectCallback = nullptr;
};

extern MenuScreen menuScreen;