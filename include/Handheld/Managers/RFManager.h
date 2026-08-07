#pragma once

// ============================================================
// BMX-Moto
// RF Manager
// ============================================================

class RFManager
{
public:
    RFManager();

    void begin();
    void update();

    void send();
        void sendHeartbeat();

private:

};