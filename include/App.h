#pragma once

#include "Common/RF/RFManager.h"
#include "Common/HeartbeatManager.h"
#include "Common/USBManager.h"
#include "Common/RaceState.h"

#include "GateNode/GateNode.h"
#include "Displays/485Display/485Display.h"
#include "Displays/GanXinDisplay/GanXinDisplay.h"
#include "Displays/MiniDisplay/MiniDisplay.h"


#include "Handheld/GUI/TFT.h"
#include "Handheld/Screens/MainScreen.h"
#include "Handheld/Screens/MenuScreen.h"
#include "Handheld/Screens/RFScreen.h"
#include "Handheld/Screens/BrightnessScreen.h"
#include "Handheld/Screens/VolumeScreen.h"
#include "Handheld/Screens/FirmwareScreen.h"
#include "Handheld/Input/TCA8418.h"
#include "Handheld/Screens/PowerOffScreen.h"

enum class Screen
{
    MAIN,
    MENU,
    RF,
    BRIGHTNESS,
    VOLUME,
    FIRMWARE,
    POWEROFF
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
    static void onHeartbeatReceived(uint8_t sender);
    static void onHeartbeatLost(uint8_t node);
    static void onHeartbeatRestored(uint8_t node);
    static void onKeyEvent(const TCA8418::KeyEvent& event);
    static void onStateChanged();
    static void onMenu();
    static void onBack();
    static void onMenuSelect(uint8_t item);
    static void onRFBack();
    static void onMenuBack();
    static void onRFData(const char* text, bool tx);
    static void onBrightnessReceived(uint8_t brightness);

    RaceState raceState;

    RFManager rf;
    HeartbeatManager heartbeat;
    USBManager usb;

    GateNode gateNode;
    Display485 display485;
    GanXinDisplay ganXinDisplay;
    MiniDisplay miniDisplay;
    
    TFTClass tft;
    MainScreen mainScreen;
    MenuScreen menuScreen;
    RFScreen rfScreen;
    BrightnessScreen brightnessScreen;
    VolumeScreen volumeScreen;
    FirmwareScreen firmwareScreen;
    PowerOffScreen powerOffScreen;

    bool splashActive = true;
    Screen activeScreen = Screen::MAIN;

    char communicationStatus[32] = "COMMUNICATIE OK";
};