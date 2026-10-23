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

void RFManager::shutdown()
{
    espNow.shutdown();
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

    if (rfDataCallback != nullptr)
    {
        char text[40];

        snprintf(
            text,
            sizeof(text),
            "Event %u Race %s %s",
            raceState->getEventSequence(),
            raceState->getRaceNumber(),
            raceState->getMode() == RaceMode::AUTO
                ? "AUTO"
                : "MANUAL"
        );

        rfDataCallback(text, true);
    }

    sendPacket(packet);
}

void RFManager::sendEvent(EventType eventType)
{
    Packet packet;

    if (!RFProtocol::createEvent(
            packet,
            nodeConfig.getNodeAddress(),
            ADDRESS_BROADCAST,
            eventType))
    {
        return;
    }

    Serial.print("RF TX Event: ");

    switch (eventType)
    {
        case EventType::GateDrop:
            Serial.println("GateDrop");
            break;

        default:
            Serial.println("Unknown");
            break;
    }

    sendPacket(packet);
}

void RFManager::sendBrightness(uint8_t brightness)
{
    Packet packet;

    if (!RFProtocol::createBrightness(
            packet,
            nodeConfig.getNodeAddress(),
            ADDRESS_BROADCAST,
            brightness))
    {
        return;
    }

    Serial.print("RF TX Brightness: ");
    Serial.print(brightness);
    Serial.println("%");

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

void RFManager::setStateChangeCallback(StateChangeCallback callback)
{
    stateChangeCallback = callback;
}

void RFManager::setBrightnessCallback(BrightnessCallback callback)
{
    brightnessCallback = callback;
}

void RFManager::setRFDataCallback(RFDataCallback callback)
{
    rfDataCallback = callback;
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
            const uint16_t eventSequence =
                packet.getEventSequence();

            const char* raceNumber =
                packet.getHeartbeatRaceNumber();

            const uint8_t modeValue =
                packet.getHeartbeatMode();

            const uint8_t sender =
                packet.getSender();

                if (heartbeatReceivedCallback != nullptr)
                {
                    heartbeatReceivedCallback(sender);
                }

            Serial.print("RX ");

            const char* senderType;

            if (sender >= 1 && sender <= 10)
            {
                senderType = "HH";
                Serial.print("HH");
            }
            else if (sender >= 11 && sender <= 20)
            {
                senderType = "GN";
                Serial.print("GN");
            }
            else if (sender >= 21 && sender <= 30)
            {
                senderType = "D";
                Serial.print("D");
            }
            else
            {
                senderType = "?";
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

            if (modeValue != static_cast<uint8_t>(RaceMode::MANUAL) &&
                modeValue != static_cast<uint8_t>(RaceMode::AUTO))
            {
                Serial.println("RF: Invalid mode");
                break;
            }

            RaceMode receivedMode =
                static_cast<RaceMode>(modeValue);

            if (rfDataCallback != nullptr)
            {
                char text[40];

                snprintf(
                    text,
                    sizeof(text),
                    "%s%u Event %u Race %s %s",
                    senderType,
                    sender,
                    eventSequence,
                    raceNumber,
                    receivedMode == RaceMode::AUTO
                        ? "AUTO"
                        : "MANUAL"
                );

                rfDataCallback(text, false);
            }

            if (raceState != nullptr)
            {
                if (raceState->applyState(
                        eventSequence,
                        raceNumber,
                        receivedMode))
                {
                    Serial.print("RF: State accepted, event ");
                    Serial.println(eventSequence);

                    if (stateChangeCallback != nullptr)
                    {
                        stateChangeCallback();
                    }
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

        case MessageType::Event:
        {
            Serial.print("RF RX: ");

            switch (packet.getEventType())
            {
                case EventType::GateDrop:
                {
                    Serial.print("GateDrop");

                    if (rfDataCallback != nullptr)
                    {
                        rfDataCallback("GateDrop", false);
                    }

                    // Alleen de HH verwerkt de GateDrop
                    // als race-event.
                    //
                    // De GateDrop zelf bevat GEEN eventSequence
                    // en GEEN RaceState.
                    //
                    // In MANUAL verandert de race niet.
                    if (raceState != nullptr &&
                        nodeConfig.isHH())
                    {
                        if (raceState->getMode() == RaceMode::AUTO)
                        {
                            raceState->gateDrop();

                            Serial.print(
                                " -> AUTO, race "
                            );

                            Serial.print(
                                raceState->getRaceNumber()
                            );

                            Serial.print(
                                ", event "
                            );

                            Serial.print(
                                raceState->getEventSequence()
                            );

                            // RaceState is lokaal gewijzigd.
                            // App zorgt voor direct scherm + RF.
                            if (stateChangeCallback != nullptr)
                            {
                                stateChangeCallback();
                            }
                        }
                        else
                        {
                            Serial.print(
                                " -> MANUAL, ignored"
                            );
                        }
                    }

                    break;
                }

                default:
                {
                    Serial.print("Unknown event");
                    break;
                }
            }

            Serial.println();

            break;
        }

        case MessageType::Brightness:
        {
            const uint8_t brightness =
                packet.getBrightness();

            Serial.print("RF RX Brightness: ");
            Serial.print(brightness);
            Serial.println("%");

            if (brightnessCallback != nullptr)
            {
                brightnessCallback(brightness);
            } 

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

void RFManager::setHeartbeatReceivedCallback(
    HeartbeatReceivedCallback callback
)
{
    heartbeatReceivedCallback = callback;
}