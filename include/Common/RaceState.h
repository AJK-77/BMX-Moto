#pragma once

// ============================================================
// BMX-Moto
// Race State
// ============================================================

#include <stdint.h>


class RaceState
{
public:
    RaceState();

    void reset();

    uint16_t getEventSequence() const;
    uint16_t getRaceNumber() const;

    void setRaceNumber(uint16_t raceNumber);

    uint16_t nextEvent();

private:
    uint16_t eventSequence = 0;
    uint16_t raceNumber = 0;
};