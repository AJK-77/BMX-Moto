#pragma once

#include <Arduino.h>

// Status LEDs — bestaande pinmapping
constexpr uint8_t PIN_LED_RED    = 32;
constexpr uint8_t PIN_LED_BLUE   = 33;
constexpr uint8_t PIN_LED_YELLOW = 25;
constexpr uint8_t PIN_LED_GREEN  = 26;

// GanXin display — SN74AHCT125N
constexpr uint8_t PIN_GX_DATA = 16;
constexpr uint8_t PIN_GX_CLK  = 17;
constexpr uint8_t PIN_GX_LE   = 18;
constexpr uint8_t PIN_GX_OE   = 19;