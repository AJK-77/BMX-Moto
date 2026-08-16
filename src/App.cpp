#include <Arduino.h>

#include "App.h"
#include "Common/NodeConfig.h"

App* App::instance = nullptr;

App::App()
{
    instance = this;
}

void App::begin()
{
    Serial.begin(115200);

    nodeConfig.begin();
    usb.begin();

    rf.setActivityCallback(onRFActivity);
    rf.begin();

    heartbeat.setRFManager(&rf);
    heartbeat.setHeartbeatCallback(onHeartbeat);
    heartbeat.begin();

    if (nodeConfig.isGateNode())
    {
        gateNode.begin();
    }
    else if (nodeConfig.isDisplay())
    {
        display485.begin();
    }
}

void App::update()
{
    usb.update();
    heartbeat.update();
    rf.update();

    if (nodeConfig.isGateNode())
    {
        gateNode.update();
    }
    else if (nodeConfig.isDisplay())
    {
        display485.update();
    }
}

void App::onRFActivity()
{
    if (instance == nullptr)
    {
        return;
    }

    if (nodeConfig.isGateNode())
    {
        instance->gateNode.onRFActivity();
    }
    else if (nodeConfig.isDisplay())
    {
        instance->display485.onRFActivity();
    }
}

void App::onHeartbeat()
{
    if (instance == nullptr)
    {
        return;
    }

    if (nodeConfig.isGateNode())
    {
        instance->gateNode.onHeartbeat();
    }
    else if (nodeConfig.isDisplay())
    {
        instance->display485.onHeartbeat();
    }
}