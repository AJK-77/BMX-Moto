#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "Common/RF/ESPNowDriver.h"

ESPNowDriver::ESPNowDriver()
{
}

bool ESPNowDriver::begin()
{
    WiFi.mode(WIFI_STA);

    Serial.print("ESP32 MAC: ");
    Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP-NOW: init failed");
        return false;
    }

    Serial.println("ESP-NOW: initialized");

    return true;
}

bool ESPNowDriver::addPeer(const uint8_t* macAddress)
{
    esp_now_peer_info_t peerInfo = {};

    memcpy(peerInfo.peer_addr, macAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_is_peer_exist(macAddress))
    {
        return true;
    }

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("ESP-NOW: add peer failed");
        return false;
    }

    Serial.println("ESP-NOW: peer added");

    return true;
}

bool ESPNowDriver::send(const uint8_t* data, uint16_t length)
{
    return false;
}

void ESPNowDriver::update()
{
}