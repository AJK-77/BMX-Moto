#include <Arduino.h>

#include "App.h"
#include "Common/NodeConfig.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/Input/TCA8418.h"

App* App::instance = nullptr;

App::App()
{
    instance = this;
}

void App::begin()
{
    Serial.begin(115200);

    // ============================================================
    // Common
    // ============================================================

    nodeConfig.begin();
    usb.begin();

    rf.setRaceState(&raceState);
    rf.setActivityCallback(onRFActivity);
    rf.setStateChangeCallback(onStateChanged);
    rf.begin();

    heartbeat.setRFManager(&rf);
    heartbeat.setHeartbeatCallback(onHeartbeat);
    heartbeat.begin();

    // ============================================================
    // GateNode
    // ============================================================

    if (nodeConfig.isGateNode())
    {
        gateNode.setRFManager(&rf);
        gateNode.setRaceState(&raceState);
        gateNode.begin();
    }

    // ============================================================
    // Display
    // ============================================================

    if (nodeConfig.isDisplay())
    {
        display485.setRaceState(&raceState);
        display485.begin();
    }

    // ============================================================
    // HandHeld
    // ============================================================

    if (nodeConfig.isHH())
    {
        TFT.begin();

        tca8418.begin();
        tca8418.setEventCallback(onKeyEvent);

        TFT.showSplash();

        mainScreen.begin();
        mainScreen.setRaceState(&raceState);
        mainScreen.setStateChangeCallback(onStateChanged);

        splashActive = true;
        activeScreen = Screen::MAIN;
    }
}

void App::update()
{
    // ============================================================
    // Common
    // ============================================================

    usb.update();
    heartbeat.update();
    rf.update();

    // ============================================================
    // GateNode
    // ============================================================

    if (nodeConfig.isGateNode())
    {
        gateNode.update();
    }

    // ============================================================
    // Display
    // ============================================================

    if (nodeConfig.isDisplay())
    {
        display485.update();
    }

    // ============================================================
    // HandHeld
    // ============================================================

    if (nodeConfig.isHH())
    {
        tca8418.update();

        if (splashActive)
        {
            if (TFT.splashFinished())
            {
                splashActive = false;
                mainScreen.draw();
            }
        }
        else
        {
            mainScreen.update();
        }
    }
}

void App::onRFActivity()
{
    if (instance == nullptr)
    {
        return;
    }

    // ============================================================
    // GateNode
    // ============================================================

    if (nodeConfig.isGateNode())
    {
        instance->gateNode.onRFActivity();
    }

    // ============================================================
    // Display
    // ============================================================

    if (nodeConfig.isDisplay())
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

    // ============================================================
    // GateNode
    // ============================================================

    if (nodeConfig.isGateNode())
    {
        instance->gateNode.onHeartbeat();
    }

    // ============================================================
    // Display
    // ============================================================

    if (nodeConfig.isDisplay())
    {
        instance->display485.onHeartbeat();
    }
}

void App::onStateChanged()
{
    if (instance == nullptr)
    {
        return;
    }

    // ============================================================
    // HandHeld
    // ============================================================

    if (nodeConfig.isHH())
    {
        // RaceState is al gewijzigd.
        // Het scherm is normaal al direct getekend door
        // MainScreen. Bij een GateDrop komt deze callback
        // rechtstreeks vanuit RFManager, dus daar tekenen
        // we het scherm hier opnieuw.
        instance->mainScreen.draw();

        // Nieuwe state direct naar alle slaves.
        instance->rf.sendHeartbeat();
    }
}

void App::onKeyEvent(const TCA8418::KeyEvent& event)
{
    if (instance == nullptr)
    {
        return;
    }

    if (event.type != TCA8418::EventType::DOWN)
    {
        return;
    }

    // ============================================================
    // Main screen
    // ============================================================

    if (instance->activeScreen == Screen::MAIN)
    {
        if (event.key == TCA8418::Key::SKL)
        {
            // Menu openen
            // Komt later weer terug wanneer MenuScreen
            // verder wordt uitgewerkt.
            return;
        }

        instance->mainScreen.onKeyEvent(event);
        return;
    }
}