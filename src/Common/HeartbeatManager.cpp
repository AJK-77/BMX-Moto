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
    // Debug (tijdelijk)
    //Serial.println("Heartbeat");
    
    if (rf != nullptr)
{
    rf->sendHeartbeat();
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