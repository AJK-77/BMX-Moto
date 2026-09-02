#include <Arduino.h>

#include "Common/RF/RFManager.h"
#include "Common/Protocol/EventType.h"
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
            raceState->getEventSequence(),
            raceState->getRaceNumber(),
            raceState->getMode()))
    {
        return;
    }

    Serial.print("TX Event ");
    Serial.println(raceState->getEventSequence());

    sendPacket(packet);
}


void RFManager::sendEvent(EventType eventType)
{
    if (raceState == nullptr)
    {
        return;
    }

    Packet packet;

    if (!RFProtocol::createEvent(
            packet,
            nodeConfig.getNodeAddress(),
            ADDRESS_BROADCAST,
            raceState->getEventSequence(),
            eventType,
            raceState->getRaceNumber()))
    {
        return;
    }

    Serial.print("RF TX: GateDrop, event ");
    Serial.println(raceState->getEventSequence());

    sendPacket(packet);
}


void RFManager::sendPacket(Packet& packet)
{
    uint8_t broadcastAddress[] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF
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
        // -----------------------------------------------------
        // Heartbeat / complete state
        // -----------------------------------------------------

        case MessageType::Heartbeat:
        {
            const uint16_t eventSequence =
                packet.getEventSequence();

            const uint16_t raceNumber =
                packet.getHeartbeatRaceNumber();

            const uint8_t modeValue =
                packet.getHeartbeatMode();

            const uint8_t sender =
                packet.getSender();


            Serial.print("RX ");

            if (sender >= 1 && sender <= 10)
            {
                Serial.print("HH");
            }
            else if (sender >= 11 && sender <= 20)
            {
                Serial.print("GN");
            }
            else if (sender >= 21 && sender <= 30)
            {
                Serial.print("D");
            }
            else
            {
                Serial.print("?");
            }

            Serial.print(sender);

            Serial.print(" Event ");
            Serial.print(eventSequence);

            Serial.print(" Race ");
            Serial.print(raceNumber);

            Serial.print(" Mode ");

            if (modeValue == static_cast<uint8_t>(RaceMode::AUTO))
            {
                Serial.println("AUTO");
            }
            else
            {
                Serial.println("MANUAL");
            }


            // -------------------------------------------------
            // Validate received mode
            // -------------------------------------------------

            if (modeValue != static_cast<uint8_t>(RaceMode::MANUAL) &&
                modeValue != static_cast<uint8_t>(RaceMode::AUTO))
            {
                Serial.println("RF: Invalid mode");
                break;
            }


            RaceMode receivedMode =
                static_cast<RaceMode>(modeValue);


            // -------------------------------------------------
            // Apply complete state when newer
            //
            // This applies to every node:
            // HH, GateNode and Display.
            //
            // Event sequence determines freshness.
            // -------------------------------------------------

            if (raceState != nullptr)
            {
                if (raceState->applyState(
                        eventSequence,
                        raceNumber,
                        receivedMode))
                {
                    Serial.print("RF: State accepted, event ");
                    Serial.println(eventSequence);
                }
                else
                {
                    Serial.print("RF: State ignored, local event ");
                    Serial.println(
                        raceState->getEventSequence());
                }
            }

            break;
        }


        // -----------------------------------------------------
        // Event
        // -----------------------------------------------------

        case MessageType::Event:
        {
            Serial.print("RF RX: ");

            switch (packet.getEventType())
            {
                case EventType::GateDrop:
                {
                    Serial.print("GateDrop");

                    if (raceState != nullptr &&
                        nodeConfig.isHH())
                    {
                        raceState->gateDrop();
                    }

                    break;
                }


                default:
                {
                    Serial.print("Unknown event");
                    break;
                }
            }

            Serial.print(", event ");
            Serial.println(packet.getEventSequence());

            break;
        }


        // -----------------------------------------------------
        // Status
        // -----------------------------------------------------

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