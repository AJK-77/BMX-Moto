#pragma once

// ============================================================
// BMX-Moto
// Heartbeat Manager
// ============================================================

class HeartbeatManager
{
public:
    HeartbeatManager();

    void begin();
    void update();
    void send();

private:
    unsigned long lastHeartbeat = 0;
};