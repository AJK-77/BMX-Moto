#pragma once

// ============================================================
// BMX-Moto
// Constants
// ============================================================

constexpr unsigned long HEARTBEAT_INTERVAL_MS = 1000;

// ============================================================
// RF Protocol
// ============================================================

constexpr uint8_t ADDRESS_BROADCAST = 255;

// Berichttypen
constexpr uint8_t MESSAGE_HEARTBEAT = 1;
constexpr uint8_t MESSAGE_EVENT = 2;
constexpr uint8_t MESSAGE_STATUS = 3;

// Header

constexpr uint8_t PACKET_VERSION = 1;