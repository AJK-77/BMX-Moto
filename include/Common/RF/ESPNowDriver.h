#pragma once

// ============================================================
// BMX-Moto
// ESP-NOW Driver
// ============================================================

#include <stdint.h>

class ESPNowDriver
{
public:
    ESPNowDriver();

    bool begin();

    bool addPeer(const uint8_t* macAddress);

    bool send(const uint8_t* data, uint16_t length);

    void update();
};