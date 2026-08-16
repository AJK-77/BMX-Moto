#pragma once

#include <stdint.h>

// ============================================================
// BMX-Moto
// 485Display Pin Configuration
// ============================================================

// Status LEDs
constexpr uint8_t PIN_LED_RED    = 32;  // Error
constexpr uint8_t PIN_LED_BLUE   = 33;  // RF
constexpr uint8_t PIN_LED_YELLOW = 25;  // Display / RS485
constexpr uint8_t PIN_LED_GREEN  = 26;  // Status / Heartbeat

// MAX485
constexpr uint8_t PIN_RS485_DE_RE = 4;
constexpr uint8_t PIN_RS485_TX    = 17;
constexpr uint8_t PIN_RS485_RX    = 16;