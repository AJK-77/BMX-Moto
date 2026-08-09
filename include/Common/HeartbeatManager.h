#pragma once

// ============================================================
// BMX-Moto
// Heartbeat Manager
// ============================================================

#include "Common/RFManager.h"

class HeartbeatManager
{
public:
    HeartbeatManager();

    void begin();
    void update();
    void send();
    void setRFManager(RFManager* manager);

private:
    unsigned long lastHeartbeat = 0;
    RFManager* rf = nullptr;
};