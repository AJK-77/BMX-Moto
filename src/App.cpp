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
    rf.setRFDataCallback(onRFData);

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
        mainScreen.setMenuCallback(onMenu);

        menuScreen.setBackCallback(onBack);
        menuScreen.setSelectCallback(onMenuSelect);
        menuScreen.begin();

        rfScreen.setBackCallback(onRFBack);
        rfScreen.begin();

        brightnessScreen.setBackCallback(onMenuBack);
        brightnessScreen.begin();

        volumeScreen.setBackCallback(onMenuBack);
        volumeScreen.begin();

        firmwareScreen.setBackCallback(onMenuBack);
        firmwareScreen.begin();

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
            if (activeScreen == Screen::MAIN)
            {
                mainScreen.update();
            }
            else if (activeScreen == Screen::MENU)
            {
                menuScreen.update();
            }
            else if (activeScreen == Screen::RF)
            {
                rfScreen.update();
            }
            else if (activeScreen == Screen::BRIGHTNESS)
            {
                brightnessScreen.update();
            }
            else if (activeScreen == Screen::VOLUME)
            {
                volumeScreen.update();
            }
            else if (activeScreen == Screen::FIRMWARE)
            {
                firmwareScreen.update();
            }
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

void App::onRFData(const char* text, bool tx)
{
    if (instance == nullptr)
    {
        return;
    }

    if (tx)
    {
        instance->rfScreen.showTx(text);
    }
    else
    {
        instance->rfScreen.showRx(text);
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

    if (nodeConfig.isHH())
    {
        if (instance->activeScreen == Screen::MAIN)
        {
            instance->mainScreen.draw();
        }

        if (instance->activeScreen == Screen::RF)
        {
            char text[40];

            snprintf(
                text,
                sizeof(text),
                "Event %u Race %s %s",
                instance->raceState.getEventSequence(),
                instance->raceState.getRaceNumber(),
                instance->raceState.getMode() == RaceMode::AUTO
                    ? "AUTO"
                    : "MANUAL"
            );

            instance->rfScreen.showTx(text);
        }

        instance->rf.sendHeartbeat();
    }

    if (nodeConfig.isDisplay())
    {
        instance->display485.onStateChanged();
    }
}

void App::onMenu()
{
    if (instance == nullptr)
    {
        return;
    }

    instance->activeScreen = Screen::MENU;
    instance->menuScreen.draw();
}

void App::onMenuSelect(uint8_t item)
{
    if (instance == nullptr)
    {
        return;
    }

    switch (item)
    {
        case 0:
            // RADIO STATUS
            instance->activeScreen = Screen::RF;
            instance->rfScreen.draw();
            break;

        case 1:
            // HELDERHEID
            instance->activeScreen = Screen::BRIGHTNESS;
            instance->brightnessScreen.draw();
            break;

        case 2:
            // GELUID
            instance->activeScreen = Screen::VOLUME;
            instance->volumeScreen.draw();
            break;

        case 3:
            // FIRMWARE
            instance->activeScreen = Screen::FIRMWARE;
            instance->firmwareScreen.draw();
            break;

        default:
            break;
    }
}

void App::onRFBack()
{
    if (instance == nullptr)
    {
        return;
    }

    instance->activeScreen = Screen::MENU;
    instance->menuScreen.draw();
}

void App::onMenuBack()
{
    if (instance == nullptr)
    {
        return;
    }

    instance->activeScreen = Screen::MENU;
    instance->menuScreen.draw();
}

void App::onBack()
{
    if (instance == nullptr)
    {
        return;
    }

    instance->activeScreen = Screen::MAIN;
    instance->mainScreen.draw();
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

    // Main screen
    if (instance->activeScreen == Screen::MAIN)
    {
        instance->mainScreen.onKeyEvent(event);
        return;
    }

    // Menu
    if (instance->activeScreen == Screen::MENU)
    {
        instance->menuScreen.onKeyEvent(event);
        return;
    }

    // RF
    if (instance->activeScreen == Screen::RF)
    {
        instance->rfScreen.onKeyEvent(event);
        return;
    }

    // Brightness
    if (instance->activeScreen == Screen::BRIGHTNESS)
    {
        instance->brightnessScreen.onKeyEvent(event);
        return;
    }

    // Volume
    if (instance->activeScreen == Screen::VOLUME)
    {
        instance->volumeScreen.onKeyEvent(event);
        return;
    }

    // Firmware
    if (instance->activeScreen == Screen::FIRMWARE)
    {
        instance->firmwareScreen.onKeyEvent(event);
        return;
    }
}