#include <Arduino.h>

#include "Handheld/Managers/RFManager.h"

#include "Common/Constants.h"

#include "Common/NodeConfig.h"

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
    Packet packet;

    packet.clear();

    packet.addHeader(NODE_ADDRESS,
                     ADDRESS_BROADCAST,
                     MESSAGE_HEARTBEAT);

    packet.addHeartbeat(millis() / 1000);

    sendPacket(packet);
}

void RFManager::sendPacket(Packet& packet)
{
    Serial.print("RF Packet - ");

    Serial.print(packet.getLength());

    Serial.println(" bytes");
}