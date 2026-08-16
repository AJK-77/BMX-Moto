#pragma once

#include <stdint.h>

class ESPNowDriver
{
public:
    ESPNowDriver();

    bool begin();

    bool addPeer(const uint8_t* macAddress);

    bool send(
        const uint8_t* macAddress,
        const uint8_t* data,
        uint16_t length
    );

    void update();

    bool available() const;

    uint16_t getReceivedLength() const;

    uint8_t* getReceivedData();

private:
    static constexpr uint16_t MAX_RECEIVE_SIZE = 250;

    uint8_t receiveBuffer[MAX_RECEIVE_SIZE];
    uint16_t receiveLength = 0;
    bool received = false;

    static void onReceive(
        const uint8_t* macAddress,
        const uint8_t* data,
        int dataLength
    );

    void handleReceive(
        const uint8_t* data,
        int dataLength
    );

    static ESPNowDriver* instance;
};