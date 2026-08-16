#include <Arduino.h>
#include <cstring>
#include <cstdlib>

#include "Common/USBManager.h"
#include "Common/NodeConfig.h"

USBManager::USBManager()
{
}

void USBManager::begin()
{
    Serial.println("USB Manager ready");
    Serial.println("Type: set address <number>");
}

void USBManager::update()
{
    if (!Serial.available())
    {
        return;
    }

    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.length() == 0)
    {
        return;
    }

    processCommand(command.c_str());
}

void USBManager::processCommand(const char* command)
{
    const char* prefix = "set address ";

    if (strncmp(command, prefix, strlen(prefix)) != 0)
    {
        Serial.println("ERROR: unknown command");
        return;
    }

    int address = atoi(command + strlen(prefix));

    if (address < 1 || address > 254)
    {
        Serial.println("ERROR: invalid address");
        return;
    }

    if (nodeConfig.saveNodeAddress(static_cast<uint8_t>(address)))
    {
        Serial.print("OK: Node address saved: ");
        Serial.println(address);
    }
    else
    {
        Serial.println("ERROR: failed to save address");
    }
}