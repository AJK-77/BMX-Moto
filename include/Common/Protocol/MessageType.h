#pragma once

// ============================================================
// BMX-Moto
// Message Types
// ============================================================

#include <stdint.h>

enum class MessageType : uint8_t
{
    Heartbeat = 1,
    Event = 2,
    Status = 3
};