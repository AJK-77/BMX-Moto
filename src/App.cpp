#include <Arduino.h>

#include "App.h"

App::App()
{
}

void App::begin()
{
    Serial.begin(115200);

    heartbeat.begin();
}

void App::update()
{
    heartbeat.update();
}