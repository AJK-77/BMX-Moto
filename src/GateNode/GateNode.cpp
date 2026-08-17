#include <Arduino.h>

#include "GateNode/GateNode.h"
#include "GateNode/Pins.h"




GateNode::GateNode()
    : leds(
        PIN_LED_RED,
        PIN_LED_BLUE,
        PIN_LED_YELLOW,
        PIN_LED_GREEN
      ),
      gateState(false),
      lastGateInput(true)
{
}


void GateNode::begin()
{
    leds.begin();
    leds.bootFlash();

    pinMode(PIN_GATE_INPUT, INPUT_PULLUP);

    gateState = false;
    lastGateInput = digitalRead(PIN_GATE_INPUT);

    leds.green(true);
}


void GateNode::update()
{
    leds.update();

    const bool gateInput = digitalRead(PIN_GATE_INPUT);

    // GateDrop = HIGH -> LOW
    if (lastGateInput == HIGH && gateInput == LOW)
{
    if (millis() - lastGateDrop >= 10000)
    {
        lastGateDrop = millis();

        gateState = true;
        leds.yellow(true);

        if (rf != nullptr)
        {
            rf->sendEvent(EventType::GateDrop);
        }
    }
}

    // Input terug naar ruststand
    if (lastGateInput == LOW && gateInput == HIGH)
    {
        gateState = false;
        leds.yellow(false);
    }

    lastGateInput = gateInput;
}


void GateNode::onRFActivity()
{
    leds.blueFlash();
}


void GateNode::onHeartbeat()
{
    leds.greenHeartbeat();
}


void GateNode::onHeartbeatReceived(uint16_t eventSequence)
{
    lastHeartbeatEvent = eventSequence;

    if (raceState != nullptr)
    {
        raceState->setEventSequence(eventSequence);
    }
}

void GateNode::setRFManager(RFManager* manager)
{
    rf = manager;
}

void GateNode::setRaceState(RaceState* state)
{
    raceState = state;
}