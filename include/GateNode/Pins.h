#pragma once

#include <stdint.h>

// ============================================================
// BMX-Moto
// GateNode Pin Configuration
// ============================================================

// Status LEDs
constexpr uint8_t PIN_LED_RED    = 32;  // Error
constexpr uint8_t PIN_LED_BLUE   = 33;  // RF
constexpr uint8_t PIN_LED_YELLOW = 25;  // Drop-Signal
constexpr uint8_t PIN_LED_GREEN  = 26;  // Status / Heartbeat

// Gate input
constexpr uint8_t PIN_GATE_INPUT = 34;