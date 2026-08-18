#pragma once

// ============================================================
// BMX-Moto
// Heartbeat Manager
// ============================================================

#include "Common/RF/RFManager.h"

class HeartbeatManager
{
public:
    using HeartbeatCallback = void (*)(void);

    HeartbeatManager();

    void begin();
    void update();
    void send();

    void setRFManager(RFManager* manager);
    void setHeartbeatCallback(HeartbeatCallback callback);

private:
    unsigned long lastHeartbeat = 0;
    HeartbeatCallback heartbeatCallback = nullptr;

    RFManager* rf = nullptr;
};