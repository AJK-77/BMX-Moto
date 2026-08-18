#pragma once

#include "Common/RF/RFManager.h"
#include "Common/HeartbeatManager.h"
#include "Common/USBManager.h"
#include "Common/RaceState.h"

#include "GateNode/GateNode.h"
#include "Displays/485Display/485Display.h"


class App
{
public:
    App();

    void begin();
    void update();

private:
    static void onRFActivity();
    static void onHeartbeat();
    static void onHeartbeatReceived(uint16_t eventSequence);

    static App* instance;

    RaceState raceState;

    RFManager rf;
    HeartbeatManager heartbeat;
    USBManager usb;

    GateNode gateNode;
    Display485 display485;
};