#include <Arduino.h>

#include "App.h"

#include "Common/NodeConfig.h"

App::App()
{
}

void App::begin()
{
    Serial.begin(115200);

    nodeConfig.begin();
    usb.begin();
    
    rf.begin();
    heartbeat.setRFManager(&rf);
    heartbeat.begin();
}

void App::update()
{
    usb.update();
    heartbeat.update();
    rf.update();
}