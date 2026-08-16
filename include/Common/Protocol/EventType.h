#pragma once

// ============================================================
// BMX-Moto
// Event Types
// ============================================================

#include <stdint.h>


enum class EventType : uint8_t
{
    Reset         = 1,
    GateDrop      = 2,
    ManualChange  = 3,
    MasterChanged = 4
};