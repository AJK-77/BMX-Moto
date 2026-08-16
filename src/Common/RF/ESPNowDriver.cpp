#include "Common/RF/ESPNowDriver.h"

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <cstring>

ESPNowDriver* ESPNowDriver::instance = nullptr;

ESPNowDriver::ESPNowDriver()
{
    instance = this;
}

bool ESPNowDriver::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Alle nodes gebruiken hetzelfde ESP-NOW kanaal.
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP-NOW: init failed");
        return false;
    }

    esp_now_register_recv_cb(onReceive);

    uint8_t broadcastAddress[] =
    {
        0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF
    };

    if (!addPeer(broadcastAddress))
    {
        Serial.println("ESP-NOW: broadcast peer failed");
        return false;
    }

    Serial.println("ESP-NOW: initialized");

    return true;
}

bool ESPNowDriver::addPeer(const uint8_t* macAddress)
{
    if (macAddress == nullptr)
    {
        return false;
    }

    if (esp_now_is_peer_exist(macAddress))
    {
        return true;
    }

    esp_now_peer_info_t peerInfo = {};

    memcpy(peerInfo.peer_addr, macAddress, 6);
    peerInfo.channel = 1;
    peerInfo.encrypt = false;

    esp_err_t result = esp_now_add_peer(&peerInfo);

    if (result != ESP_OK)
    {
        Serial.print("ESP-NOW: add peer failed: ");
        Serial.println(result);
        return false;
    }

    return true;
}

bool ESPNowDriver::send(
    const uint8_t* macAddress,
    const uint8_t* data,
    uint16_t length)
{
    if (macAddress == nullptr || data == nullptr || length == 0)
    {
        return false;
    }

    if (!esp_now_is_peer_exist(macAddress))
    {
        if (!addPeer(macAddress))
        {
            return false;
        }
    }

    esp_err_t result = esp_now_send(
        macAddress,
        data,
        length
    );

    if (result != ESP_OK)
    {
        Serial.print("ESP-NOW: send failed: ");
        Serial.println(result);
        return false;
    }

    return true;
}

void ESPNowDriver::update()
{
}

void ESPNowDriver::onReceive(
    const uint8_t* macAddress,
    const uint8_t* data,
    int dataLength)
{
    if (instance != nullptr)
    {
        instance->handleReceive(data, dataLength);
    }
}

void ESPNowDriver::handleReceive(
    const uint8_t* data,
    int dataLength)
{
    if (data == nullptr || dataLength <= 0)
    {
        return;
    }

    if (dataLength > MAX_RECEIVE_SIZE)
    {
        return;
    }

    memcpy(
        receiveBuffer,
        data,
        dataLength
    );

    receiveLength = dataLength;
    received = true;
}

bool ESPNowDriver::available() const
{
    return received;
}

uint16_t ESPNowDriver::getReceivedLength() const
{
    return receiveLength;
}

uint8_t* ESPNowDriver::getReceivedData()
{
    received = false;

    return receiveBuffer;
}