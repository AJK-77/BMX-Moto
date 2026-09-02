#pragma once

#include "Common/RF/RFManager.h"
#include "Common/HeartbeatManager.h"
#include "Common/USBManager.h"
#include "Common/RaceState.h"

#include "GateNode/GateNode.h"

#include "Displays/485Display/485Display.h"

#include "Handheld/GUI/TFT.h"
#include "Handheld/Screens/MainScreen.h"
#include "Handheld/Input/TCA8418.h"

enum class Screen
{
    MAIN,
    MENU
};

class App
{
public:
    App();

    void begin();
    void update();

private:
    static App* instance;

    static void onRFActivity();
    static void onHeartbeat();
    static void onKeyEvent(
        const TCA8418::KeyEvent& event
    );

    static void onStateChanged();

    RaceState raceState;

    RFManager rf;
    HeartbeatManager heartbeat;
    USBManager usb;

    GateNode gateNode;
    Display485 display485;

    TFTClass tft;
    MainScreen mainScreen;

    bool splashActive = true;
    Screen activeScreen = Screen::MAIN;
};