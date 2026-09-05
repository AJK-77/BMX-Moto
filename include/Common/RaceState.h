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
    const char* getRaceNumber() const;
    RaceMode getMode() const;

    void setRaceNumber(const char* raceNumber);
    void setMode(RaceMode mode);

    uint16_t nextEvent();
    void gateDrop();

    void setEventSequence(uint16_t eventSequence);

    // Apply a complete state when the received event sequence is newer.
    bool applyState(
        uint16_t eventSequence,
        const char* raceNumber,
        RaceMode mode
    );

private:
    uint16_t eventSequence = 0;

    // Race is ALWAYS exactly four characters.
    // Valid characters: '0' - '9' or ' '.
    char raceNumber[5] = "0000";

    RaceMode mode = RaceMode::MANUAL;
};