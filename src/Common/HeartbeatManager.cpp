#include <Arduino.h>

#include "Common/Constants.h"
#include "Common/HeartbeatManager.h"


HeartbeatManager::HeartbeatManager()
{
}


void HeartbeatManager::begin()
{
    lastHeartbeat = millis();
}

void HeartbeatManager::shutdown()
{
    rf = nullptr;
}

void HeartbeatManager::send()
{
    if (rf != nullptr)
    {
        rf->sendHeartbeat();
    }

    if (heartbeatCallback != nullptr)
    {
        heartbeatCallback();
    }
}


void HeartbeatManager::update()
{
    const unsigned long now = millis();
    
    if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL_MS)
    {
        lastHeartbeat = millis();

        send();
    }

    for (uint16_t address = 1;
     address <= MAX_NODE_ADDRESS;
     address++)
{
    if (lastReceivedHeartbeat[address] == 0)
    {
        continue;
    }

    if (!nodeLost[address])
    {
        const unsigned long timeout =
            static_cast<unsigned long>(
                HEARTBEAT_TIMEOUT_COUNT
            ) * HEARTBEAT_INTERVAL_MS;

        if (now - lastReceivedHeartbeat[address] >= timeout)
        {
            Serial.print("HEARTBEAT LOST: node ");
            Serial.println(address);

            nodeLost[address] = true;

            if (heartbeatLostCallback != nullptr)
            {
                heartbeatLostCallback(static_cast<uint8_t>(address));
            }
        }
    }
}

}


void HeartbeatManager::setRFManager(RFManager* manager)
{
    rf = manager;
}

void HeartbeatManager::setHeartbeatCallback(HeartbeatCallback callback)
{
    heartbeatCallback = callback;
}

void HeartbeatManager::onHeartbeatReceived(uint8_t sender)
{
    if (sender == 0 || sender > MAX_NODE_ADDRESS)
    {
        return;
    }

    lastReceivedHeartbeat[sender] = millis();

    if (nodeLost[sender])
    {
        nodeLost[sender] = false;

        Serial.print("HEARTBEAT RESTORED: node ");
        Serial.println(sender);

        if (heartbeatRestoredCallback != nullptr)
        {
            heartbeatRestoredCallback(sender);
        }
    }

    if (heartbeatReceivedCallback != nullptr)
    {
        heartbeatReceivedCallback(sender);
    }
}

void HeartbeatManager::setHeartbeatLostCallback(
    HeartbeatLostCallback callback
)
{
    heartbeatLostCallback = callback;
}

void HeartbeatManager::setHeartbeatRestoredCallback(
    HeartbeatRestoredCallback callback
)
{
    heartbeatRestoredCallback = callback;
}