#pragma once

#include <Arduino.h>

namespace DisplayProtocol
{
    constexpr uint8_t HEADER_1 = 0x04;
    constexpr uint8_t HEADER_2 = 0x04;
    constexpr uint8_t MESSAGE_TYPE = 0x81;
    constexpr uint8_t DISPLAY_DATA = 0x3D;

    constexpr uint8_t SEPARATOR = 0x20;

    constexpr uint8_t DATA_END = 0x03;
    constexpr uint8_t FRAME_END_1 = 0x06;
    constexpr uint8_t FRAME_END_2 = 0x06;

    constexpr uint8_t MANCHE_BASE = 0xB0;
    constexpr uint8_t CHECKSUM_XOR = 0x1D;

    constexpr size_t DISPLAY_FRAME_SIZE = 13;

    void buildDisplayFrame(
        uint16_t raceNumber,
        uint8_t* frame
    );
}