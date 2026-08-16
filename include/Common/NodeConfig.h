#pragma once

#include <stdint.h>

enum class HardwareType : uint8_t
{
    Unknown = 0,
    HH = 1,
    GateNode = 2,
    Display = 3
};

class NodeConfig
{
public:
    NodeConfig();

    bool begin();
    bool saveNodeAddress(uint8_t address);

    uint8_t getNodeAddress() const;
    HardwareType getHardwareType() const;

    bool isHH() const;
    bool isGateNode() const;
    bool isDisplay() const;

private:
    uint8_t nodeAddress;
    bool valid;
    HardwareType hardwareType;

    void determineHardwareType();
};

extern NodeConfig nodeConfig;