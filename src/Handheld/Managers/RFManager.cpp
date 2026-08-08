#include <Arduino.h>

#include "Handheld/Managers/RFManager.h"

RFManager::RFManager()
{
}

void RFManager::begin()
{
}

void RFManager::update()
{
}

void RFManager::send()
{
    // TODO:
}

void RFManager::sendHeartbeat()
{
    // TODO:
    // Heartbeat-blok toevoegen aan pakket

    sendPacket();
}

void RFManager::sendPacket()
{
    Serial.println("RF Packet");
}