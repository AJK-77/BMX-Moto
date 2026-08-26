#include <Arduino.h>
#include "Displays/485Display/485Display.h"
#include "Displays/485Display/Pins.h"
#include "Displays/485Display/DisplayProtocol.h"

Display485::Display485()
    : leds(
        PIN_LED_RED,
        PIN_LED_BLUE,
        PIN_LED_YELLOW,
        PIN_LED_GREEN
      )
{
}

void Display485::begin()
{
    pinMode(PIN_RS485_DE_RE, OUTPUT);

    // MAX485 in receive mode
    digitalWrite(PIN_RS485_DE_RE, LOW);

    leds.begin();
    leds.bootFlash();

    // Display node actief
    leds.green(true);

    Serial2.begin(
        9600,
        SERIAL_8N1,
        PIN_RS485_RX,
        PIN_RS485_TX
    );

    Serial.println("RS485: initialized");
}

void Display485::update()
{
    leds.update();
    // RS485 communicatie komt hier.
}

void Display485::onRFActivity()
{
    leds.blueFlash();
}

void Display485::onHeartbeat()
{
    leds.greenHeartbeat();

    sendDisplayFrame();
}

void Display485::onHeartbeatReceived(uint16_t eventSequence)
{
    lastHeartbeatEvent = eventSequence;

    if (raceState != nullptr)
    {
        raceState->setEventSequence(eventSequence);
    }
}

void Display485::setRaceState(RaceState* state)
{
    raceState = state;
}


void Display485::sendDisplayFrame()
{
    if (raceState == nullptr)
    {
        return;
    }

    uint8_t frame[DisplayProtocol::DISPLAY_FRAME_SIZE];

    DisplayProtocol::buildDisplayFrame(
        raceState->getRaceNumber(),
        frame
    );

    digitalWrite(PIN_RS485_DE_RE, HIGH);

    delay(2);

    Serial2.write(
        frame,
        sizeof(frame)
    );

    Serial2.flush();

    delay(2);

    digitalWrite(PIN_RS485_DE_RE, LOW);
}