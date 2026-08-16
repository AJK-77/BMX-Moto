#include <Arduino.h>
#include <Preferences.h>

#include "Common/NodeConfig.h"

namespace
{
    constexpr const char* NVS_NAMESPACE = "bmxnode";
    constexpr const char* KEY_ADDRESS = "address";

    constexpr uint8_t DEFAULT_NODE_ADDRESS = 1;
}

NodeConfig nodeConfig;


NodeConfig::NodeConfig()
    : nodeAddress(DEFAULT_NODE_ADDRESS),
      valid(false),
      hardwareType(HardwareType::Unknown)
{
}


bool NodeConfig::begin()
{
    Preferences preferences;

    if (!preferences.begin(NVS_NAMESPACE, true))
    {
        Serial.println("NodeConfig: no configuration found");

        valid = false;
        hardwareType = HardwareType::Unknown;

        return false;
    }

    if (!preferences.isKey(KEY_ADDRESS))
    {
        Serial.println("NodeConfig: no configuration found");

        preferences.end();

        valid = false;
        hardwareType = HardwareType::Unknown;

        return false;
    }

    nodeAddress = preferences.getUChar(
        KEY_ADDRESS,
        DEFAULT_NODE_ADDRESS
    );

    preferences.end();

    valid = true;

    determineHardwareType();

    Serial.print("NodeConfig: address = ");
    Serial.println(nodeAddress);

    switch (hardwareType)
    {
        case HardwareType::HH:
            Serial.println("NodeConfig: hardware = HandHeld");
            break;

        case HardwareType::GateNode:
            Serial.println("NodeConfig: hardware = GateNode");
            break;

        case HardwareType::Display:
            Serial.println("NodeConfig: hardware = Display");
            break;

        default:
            Serial.println("NodeConfig: hardware = Unknown");
            break;
    }

    return true;
}


bool NodeConfig::saveNodeAddress(uint8_t address)
{
    Preferences preferences;

    if (!preferences.begin(NVS_NAMESPACE, false))
    {
        Serial.println("NodeConfig: NVS open for write failed");
        return false;
    }

    size_t written = preferences.putUChar(
        KEY_ADDRESS,
        address
    );

    preferences.end();

    if (written != sizeof(uint8_t))
    {
        Serial.println("NodeConfig: address save failed");
        return false;
    }

    nodeAddress = address;
    valid = true;

    determineHardwareType();

    Serial.print("NodeConfig: address saved = ");
    Serial.println(nodeAddress);

    return true;
}


uint8_t NodeConfig::getNodeAddress() const
{
    return nodeAddress;
}


HardwareType NodeConfig::getHardwareType() const
{
    return hardwareType;
}


bool NodeConfig::isHH() const
{
    return hardwareType == HardwareType::HH;
}


bool NodeConfig::isGateNode() const
{
    return hardwareType == HardwareType::GateNode;
}


bool NodeConfig::isDisplay() const
{
    return hardwareType == HardwareType::Display;
}


void NodeConfig::determineHardwareType()
{
    if (nodeAddress >= 1 && nodeAddress <= 10)
    {
        hardwareType = HardwareType::HH;
    }
    else if (nodeAddress >= 11 && nodeAddress <= 20)
    {
        hardwareType = HardwareType::GateNode;
    }
    else if (nodeAddress >= 21 && nodeAddress <= 30)
    {
        hardwareType = HardwareType::Display;
    }
    else
    {
        hardwareType = HardwareType::Unknown;
    }
}