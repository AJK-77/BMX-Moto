#pragma once

#include <stdint.h>

enum class RaceMode
{
    MANUAL,
    AUTO
};

class RaceState
{
public:
    RaceState();

    void reset();

    uint16_t getEventSequence() const;
    uint16_t getRaceNumber() const;
    RaceMode getMode() const;

    void setRaceNumber(uint16_t raceNumber);
    void setMode(RaceMode mode);

    uint16_t nextEvent();
    void gateDrop();

    void setEventSequence(uint16_t eventSequence);

    // Apply a complete state when the received event sequence is newer.
    bool applyState(uint16_t eventSequence,
                    uint16_t raceNumber,
                    RaceMode mode);

private:
    uint16_t eventSequence = 0;
    uint16_t raceNumber = 0;
    RaceMode mode = RaceMode::MANUAL;
};