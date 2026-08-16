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
    if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL_MS)
    {
        lastHeartbeat = millis();

        send();
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