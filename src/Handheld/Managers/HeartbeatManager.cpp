#include <Arduino.h>

#include "Common/Constants.h"
#include "Handheld/Managers/HeartbeatManager.h"

HeartbeatManager::HeartbeatManager()
{
}

void HeartbeatManager::begin()
{
    lastHeartbeat = millis();
}

void HeartbeatManager::send()
{
    Serial.println("Heartbeat");
    
    // TODO:
    // Build en verzend Heartbeat
}

void HeartbeatManager::update()
{
    if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL_MS)
    {
        lastHeartbeat = millis();

        send();
    }
}