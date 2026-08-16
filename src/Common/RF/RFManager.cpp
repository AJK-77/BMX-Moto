#include <Arduino.h>

#include "Common/RF/RFManager.h"

#include "Common/Constants.h"
#include "Common/NodeConfig.h"
#include "Common/Protocol/RFProtocol.h"


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

    Packet packet;

    if (!packet.load(data, length))
    {
        return;
    }

    processPacket(packet);
}


void RFManager::send()
{
}


void RFManager::sendHeartbeat()
{
    if (raceState == nullptr)
    {
        return;
    }

    Packet packet;

    if (!RFProtocol::createHeartbeat(
            packet,
            nodeConfig.getNodeAddress(),
            raceState->getEventSequence()))
    {
        return;
    }

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


void RFManager::setRaceState(RaceState* state)
{
    raceState = state;
}


void RFManager::processPacket(Packet& packet)
{
    if (!RFProtocol::validatePacket(packet))
    {
        return;
    }

    switch (packet.getMessageType())
    {
        case MessageType::Heartbeat:
        {
            break;
        }

        case MessageType::Event:
        {
            break;
        }

        case MessageType::Status:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}


void RFManager::activity()
{
    if (activityCallback != nullptr)
    {
        activityCallback();
    }
}