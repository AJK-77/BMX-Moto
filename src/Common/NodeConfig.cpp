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
      valid(false)
{
}

bool NodeConfig::begin()
{
    Preferences preferences;

    if (!preferences.begin(NVS_NAMESPACE, true))
    {
        Serial.println("NodeConfig: no configuration found");

        valid = false;
        return false;
    }

    if (!preferences.isKey(KEY_ADDRESS))
    {
        Serial.println("NodeConfig: no configuration found");

        preferences.end();

        valid = false;
        return false;
    }

    nodeAddress = preferences.getUChar(
        KEY_ADDRESS,
        DEFAULT_NODE_ADDRESS);

    preferences.end();

    valid = true;

    Serial.print("NodeConfig: address = ");
    Serial.println(nodeAddress);

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

    size_t written = preferences.putUChar(KEY_ADDRESS, address);

    preferences.end();

    if (written != sizeof(uint8_t))
    {
        Serial.println("NodeConfig: address save failed");
        return false;
    }

    nodeAddress = address;
    valid = true;

    Serial.print("NodeConfig: address saved = ");
    Serial.println(nodeAddress);

    return true;
}

uint8_t NodeConfig::getNodeAddress() const
{
    return nodeAddress;
}