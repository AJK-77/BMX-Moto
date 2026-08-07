#pragma once

// ============================================================
// BMX-Moto
// Handheld Application
// ============================================================

#include "Handheld/Managers/HeartbeatManager.h"
#include "Handheld/Managers/RFManager.h"

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
    HeartbeatManager heartbeat;
    // EventManager     event;
    // StatusManager    status;
    RFManager        rf;
    // BatteryManager   battery;
    // SettingsManager  settings;
};