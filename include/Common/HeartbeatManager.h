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
    using HeartbeatReceivedCallback = void (*)(uint8_t sender);
    using HeartbeatLostCallback = void (*)(uint8_t node);
    using HeartbeatRestoredCallback = void (*)(uint8_t node);
    

    HeartbeatManager();

    void begin();
    void update();
    void send();
    void shutdown();

    void setRFManager(RFManager* manager);
    void setHeartbeatCallback(HeartbeatCallback callback);
    void setHeartbeatLostCallback(HeartbeatLostCallback callback);
    void setHeartbeatRestoredCallback(HeartbeatRestoredCallback callback);

    void onHeartbeatReceived(uint8_t sender);

private:
    unsigned long lastHeartbeat = 0;
    HeartbeatCallback heartbeatCallback = nullptr;
    HeartbeatReceivedCallback heartbeatReceivedCallback = nullptr;
    HeartbeatLostCallback heartbeatLostCallback = nullptr;
    HeartbeatRestoredCallback heartbeatRestoredCallback = nullptr;

    RFManager* rf = nullptr;

    static constexpr uint8_t MAX_NODE_ADDRESS = 254;
    static constexpr uint8_t HEARTBEAT_TIMEOUT_COUNT = 6;

    unsigned long lastReceivedHeartbeat[MAX_NODE_ADDRESS + 1] = {};
    bool nodeLost[MAX_NODE_ADDRESS + 1] = {};
};