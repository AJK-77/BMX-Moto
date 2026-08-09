#pragma once

// ============================================================
// BMX-Moto
// Node Configuration
// ============================================================

#include <stdint.h>

class NodeConfig
{
public:
    NodeConfig();

    bool begin();
    bool saveNodeAddress(uint8_t address);

    uint8_t getNodeAddress() const;

private:
    uint8_t nodeAddress;
    bool valid;
};

extern NodeConfig nodeConfig;