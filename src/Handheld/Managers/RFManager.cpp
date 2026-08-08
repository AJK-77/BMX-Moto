#include <Arduino.h>

#include "Handheld/Managers/RFManager.h"

#include "Common/Constants.h"
#include "Common/NodeConfig.h"
#include "Common/Protocol/Packet.h"
#include "Common/Protocol/MessageType.h"

RFManager::RFManager()
{
}

bool RFManager::begin()
{
    return espNow.begin();
}

void RFManager::update()
{
    espNow.update();
}

void RFManager::send()
{
    // TODO:
}

void RFManager::sendHeartbeat()
{
    Packet packet;

    packet.clear();

    packet.addHeader(
    nodeConfig.getNodeAddress(),
    ADDRESS_BROADCAST,
    MessageType::Heartbeat);

    packet.addHeartbeat(millis() / 1000);

    packet.addCRC();

    sendPacket(packet);
}

void RFManager::sendPacket(Packet& packet)
{
    // TODO:
    // ESP-NOW verzending volgt in de volgende stap.

    Serial.print("RF Packet - ");
    Serial.print(packet.getLength());
    Serial.println(" bytes");
}