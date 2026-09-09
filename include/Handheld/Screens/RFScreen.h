#pragma once

#include "Handheld/Input/TCA8418.h"

class RFScreen
{
public:
    using BackCallback = void (*)();

    RFScreen();

    void begin();
    void draw();
    void update();

    void setBackCallback(BackCallback callback);
    void onKeyEvent(const TCA8418::KeyEvent& event);

    void showTx(const char* text);
    void showRx(const char* text);

private:
    static constexpr uint8_t MAX_LINES = 5;
    static constexpr uint8_t MAX_LINE_LENGTH = 39;

    char lines[MAX_LINES][MAX_LINE_LENGTH + 1];
    bool dataChanged = false;
    BackCallback backCallback = nullptr;

    void addLine(const char* prefix, const char* text);
};