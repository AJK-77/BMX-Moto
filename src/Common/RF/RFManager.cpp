#include <Arduino.h>

#include "Common/RF/RFManager.h"

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
    if (!espNow.available())
    {
        return;
    }

    uint16_t length = espNow.getReceivedLength();
    uint8_t* data = espNow.getReceivedData();

    if (data == nullptr || length == 0)
    {
        return;
    }

    activity();
}

void RFManager::send()
{
}

void RFManager::sendHeartbeat()
{
    Packet packet;

    packet.clear();

    packet.addHeader(
        nodeConfig.getNodeAddress(),
        ADDRESS_BROADCAST,
        MessageType::Heartbeat
    );

    packet.addHeartbeat(millis() / 1000);
    packet.addCRC();

    sendPacket(packet);
}

void RFManager::sendPacket(Packet& packet)
{
    uint8_t broadcastAddress[] =
    {
        0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF
    };

    if (espNow.send(
        broadcastAddress,
        packet.getData(),
        packet.getLength()))
    {
        activity();
    }
}

void RFManager::setActivityCallback(ActivityCallback callback)
{
    activityCallback = callback;
}

void RFManager::activity()
{
    if (activityCallback != nullptr)
    {
        activityCallback();
    }
}