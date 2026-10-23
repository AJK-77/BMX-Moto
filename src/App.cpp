#include <Arduino.h>
#include <esp_sleep.h>

#include "App.h"
#include "Common/NodeConfig.h"
#include "Handheld/Pins.h"
#include "Handheld/GUI/TFT.h"
#include "Handheld/Input/TCA8418.h"
#include "Handheld/Screens/PowerOffScreen.h"
#include "Handheld/Audio.h"
#include "Displays/GanXinDisplay/GanXinDisplay.h"
#include "Displays/GanXinDisplay/Pins.h"


App* App::instance = nullptr;

bool powerButtonHeld = false;
unsigned long powerButtonStart = 0;
bool powerOffTriggered = false;
bool powerDownMode = false;

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
    rf.setHeartbeatReceivedCallback(onHeartbeatReceived);
    rf.setBrightnessCallback(onBrightnessReceived);
    
    rf.begin();

    heartbeat.setRFManager(&rf);
    heartbeat.setHeartbeatCallback(onHeartbeat);
    heartbeat.setHeartbeatLostCallback(onHeartbeatLost);
    heartbeat.setHeartbeatRestoredCallback(onHeartbeatRestored);
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

    if (nodeConfig.isGanXinDisplay())
    {
        ganXinDisplay.setRaceState(&raceState);
        ganXinDisplay.begin();
    }

    // ============================================================
    // MiniDisplay
    // ============================================================

    if (nodeConfig.isMiniDisplay())
    {
        miniDisplay.setRaceState(&raceState);
        miniDisplay.begin();
    }

    // ============================================================
    // HandHeld
    // ============================================================

    if (nodeConfig.isHH())
    {
        pinMode(POWER_WAKE_PIN, INPUT);

        Audio.begin();

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
        brightnessScreen.setRFManager(&rf);

        volumeScreen.setBackCallback(onMenuBack);
        volumeScreen.begin();

        firmwareScreen.setBackCallback(onMenuBack);
        firmwareScreen.begin();

        powerOffScreen.setBackCallback([]()
        {
            instance->activeScreen = Screen::MAIN;
            instance->mainScreen.draw();
        });

        powerOffScreen.setPowerDownCallback([]()
        {
            powerDownMode = true;
            instance->heartbeat.shutdown();
            instance->rf.shutdown();
        });

        splashActive = true;
        activeScreen = Screen::MAIN;     
        
        Audio.boot();
    }
}

void App::update()
{
    if (nodeConfig.isHH() && powerDownMode)
    {
        if (digitalRead(POWER_WAKE_PIN) == LOW)
        {
            ESP.restart();
        }

        esp_sleep_enable_ext0_wakeup(
            GPIO_NUM_35,
            0
        );

        esp_deep_sleep_start();
    }

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

    if (nodeConfig.isGanXinDisplay())
    {
        ganXinDisplay.update();
    }

    if (nodeConfig.isMiniDisplay())
    {
        miniDisplay.update();
    }

    // ============================================================
    // HandHeld
    // ============================================================

    if (nodeConfig.isHH())
    {

        bool powerPressed = (digitalRead(POWER_WAKE_PIN) == LOW);

        if (powerPressed && !powerButtonHeld)
        {
            powerButtonHeld = true;
            powerButtonStart = millis();
            powerOffTriggered = false;
        }
        else if (!powerPressed && powerButtonHeld)
        {
            powerButtonHeld = false;
            powerButtonStart = 0;
            powerOffTriggered = false;
        }

        if (powerButtonHeld && !powerOffTriggered)
        {
            if (millis() - powerButtonStart >= 1000)
            {
                powerOffTriggered = true;
                activeScreen = Screen::POWEROFF;
                powerOffScreen.draw();
            }
        }

        tca8418.update();
        

        if (splashActive)
        {

            if (TFT.splashFinished())
            {
                splashActive = false;

                TFT.clearFull();
                mainScreen.draw();
                TFT.drawStatusBar("COMMUNICATIE OK");
            }
        }
        else
        {

            TFT.drawBattery(
                tca8418.battery25(),
                tca8418.battery50(),
                tca8418.battery75(),
                tca8418.battery100()
            );

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
            else if (activeScreen == Screen::POWEROFF)
            {
                powerOffScreen.update();
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

        if (nodeConfig.isGanXinDisplay())
    {
        instance->ganXinDisplay.onRFActivity();
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

        if (nodeConfig.isGanXinDisplay())
    {
        instance->ganXinDisplay.onHeartbeat();
    }
}

void App::onHeartbeatReceived(uint8_t sender)
{
    if (instance == nullptr)
    {
        return;
    }

    instance->heartbeat.onHeartbeatReceived(sender);
}

void App::onHeartbeatLost(uint8_t node)
{

    Serial.print("APP HEARTBEAT LOST: node ");
    Serial.println(node);

    if (instance == nullptr)
    {
        return;
    }

    if (nodeConfig.isGateNode())
    {
        instance->gateNode.onHeartbeatLost(node);
    }

    if (nodeConfig.isDisplay())
    {
        instance->display485.onHeartbeatLost(node);
    }

        if (nodeConfig.isGanXinDisplay())
    {
        instance->ganXinDisplay.onHeartbeatLost(node);
    }

    if (nodeConfig.isMiniDisplay())
    {
        instance->miniDisplay.onHeartbeatLost(node);
    }

    if (nodeConfig.isHH())
    {
        char statusText[32];

        if (node >= 11 && node <= 20)
        {
            snprintf(
                statusText,
                sizeof(statusText),
                "LOST: Gate %u",
                node - 10
            );
        }
        else if (node >= 21 && node <= 30)
        {
            snprintf(
                statusText,
                sizeof(statusText),
                "LOST: Display %u",
                node - 20
            );
        }
        else
        {
            snprintf(
                statusText,
                sizeof(statusText),
                "LOST: Node %u",
                node
            );
        }

        if (!instance->splashActive)
        {
            TFT.drawStatusBar(statusText);
        }
    }   // ← deze sluit if (nodeConfig.isHH())

}

void App::onHeartbeatRestored(uint8_t node)
{
    if (instance == nullptr)
    {
        return;
    }

    if (nodeConfig.isGateNode())
    {
        instance->gateNode.onHeartbeatRestored();
    }

    if (nodeConfig.isDisplay())
    {
        instance->display485.onHeartbeatRestored();
    }

        if (nodeConfig.isGanXinDisplay())
    {
        instance->ganXinDisplay.onHeartbeatRestored();
    }

    if (nodeConfig.isMiniDisplay())
    {
        instance->miniDisplay.onHeartbeatRestored();
    }

    if (nodeConfig.isHH())
    {
        strncpy(
            instance->communicationStatus,
            "COMMUNICATIE OK",
            sizeof(instance->communicationStatus) - 1
        );

        instance->communicationStatus[
            sizeof(instance->communicationStatus) - 1
        ] = '\0';

        //if (!instance->splashActive)
        {
            TFT.drawStatusBar(instance->communicationStatus);
        }
    }
}

void App::onBrightnessReceived(uint8_t brightness)
{
    if (instance == nullptr)
    {
        return;
    }

    if (nodeConfig.isGanXinDisplay())
    {
        instance->ganXinDisplay.setBrightness(brightness);
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
        if (instance->activeScreen == Screen::MAIN &&
            !instance->splashActive)
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

        if (nodeConfig.isGanXinDisplay())
    {
        instance->ganXinDisplay.onStateChanged();
    }

    if (nodeConfig.isMiniDisplay())
    {
        instance->miniDisplay.onStateChanged();
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

    if (event.type == TCA8418::EventType::DOWN)
    {
        Audio.click(instance->volumeScreen.getVolume());
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

    // Power Off
    if (instance->activeScreen == Screen::POWEROFF)
    {
        instance->powerOffScreen.onKeyEvent(event);
        return;
    }
}