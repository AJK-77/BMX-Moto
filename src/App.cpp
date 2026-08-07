#include <Arduino.h>

#include "App.h"

App::App()
{
}

void App::begin()
{
    Serial.begin(115200);

    rf.begin();
    heartbeat.setRFManager(&rf);
    heartbeat.begin();

}

void App::update()
{
    heartbeat.update();
    rf.update();
}