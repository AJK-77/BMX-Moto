#pragma once

// ============================================================
// BMX-Moto
// Handheld Application
// ============================================================

#include "Common/USBService.h"
#include "Common/HeartbeatManager.h"
#include "Common/RFManager.h"


class App
{
public:
    App();

    // Initialisatie
    void begin();
    

    // Hoofdprogramma
    void update();

private:

    // ========================================================
    // Managers
    // ========================================================

    // Worden later toegevoegd
    USBService       usb;
    HeartbeatManager heartbeat;
    // EventManager     event;
    // StatusManager    status;
    RFManager        rf;
    // BatteryManager   battery;
    // SettingsManager  settings;
};