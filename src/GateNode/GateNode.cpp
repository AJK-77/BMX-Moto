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
      gateState(false)
{
}

void GateNode::begin()
{
    leds.begin();
    leds.bootFlash();

    gateState = false;
    leds.green(true);
}

void GateNode::update()
{
    leds.update();
}

void GateNode::onRFActivity()
{
    leds.blueFlash();
}

void GateNode::onHeartbeat()
{
    leds.greenHeartbeat();
}