#include "Common/RaceState.h"

#include <string.h>


RaceState::RaceState()
{
    reset();
}


void RaceState::reset()
{
    eventSequence = 0;

    raceNumber[0] = '0';
    raceNumber[1] = '0';
    raceNumber[2] = '0';
    raceNumber[3] = '0';
    raceNumber[4] = '\0';

    mode = RaceMode::MANUAL;
}


uint16_t RaceState::getEventSequence() const
{
    return eventSequence;
}


const char* RaceState::getRaceNumber() const
{
    return raceNumber;
}


RaceMode RaceState::getMode() const
{
    return mode;
}


void RaceState::setRaceNumber(const char* value)
{
    if (value == nullptr)
    {
        return;
    }

    // Always start with four blanks.
    raceNumber[0] = ' ';
    raceNumber[1] = ' ';
    raceNumber[2] = ' ';
    raceNumber[3] = ' ';
    raceNumber[4] = '\0';

    // Copy at most four valid characters.
    uint8_t position = 0;

    while (value[position] != '\0' && position < 4)
    {
        char c = value[position];

        if ((c >= '0' && c <= '9') || c == ' ')
        {
            raceNumber[position] = c;
            position++;
        }
        else
        {
            return;
        }
    }
}


void RaceState::setMode(RaceMode value)
{
    mode = value;
}


uint16_t RaceState::nextEvent()
{
    if (eventSequence >= 9999)
    {
        eventSequence = 0;
    }
    else
    {
        eventSequence++;
    }

    return eventSequence;
}


void RaceState::gateDrop()
{
    nextEvent();

    // Race is:
    //
    // [Manche][Moto][Moto][Moto]
    //
    // Only the last three digits change.
    //
    // Example:
    // 0125 -> 0126
    // 0999 -> 0000
    // 1999 -> 1000

    uint8_t manche = 0;
    uint16_t moto = 0;

    // Manche
    if (raceNumber[0] >= '0' &&
        raceNumber[0] <= '9')
    {
        manche = raceNumber[0] - '0';
    }

    // Moto
    if (raceNumber[1] >= '0' &&
        raceNumber[1] <= '9')
    {
        moto += (raceNumber[1] - '0') * 100;
    }

    if (raceNumber[2] >= '0' &&
        raceNumber[2] <= '9')
    {
        moto += (raceNumber[2] - '0') * 10;
    }

    if (raceNumber[3] >= '0' &&
        raceNumber[3] <= '9')
    {
        moto += raceNumber[3] - '0';
    }

    moto++;

    if (moto > 999)
    {
        moto = 0;
    }

    raceNumber[0] = '0' + manche;
    raceNumber[1] = '0' + (moto / 100);
    raceNumber[2] = '0' + ((moto / 10) % 10);
    raceNumber[3] = '0' + (moto % 10);
    raceNumber[4] = '\0';
}


void RaceState::setEventSequence(uint16_t value)
{
    eventSequence = value;

    if (eventSequence == 0)
    {
        mode = RaceMode::MANUAL;
    }
}


bool RaceState::applyState(
    uint16_t receivedEventSequence,
    const char* receivedRaceNumber,
    RaceMode receivedMode)
{
    if (receivedEventSequence <= eventSequence)
    {
        return false;
    }

    if (receivedRaceNumber == nullptr)
    {
        return false;
    }

    // Race must contain exactly four valid characters.
    for (uint8_t i = 0; i < 4; i++)
    {
        char c = receivedRaceNumber[i];

        if (!((c >= '0' && c <= '9') || c == ' '))
        {
            return false;
        }
    }

    // Fifth character must be the string terminator.
    if (receivedRaceNumber[4] != '\0')
    {
        return false;
    }

    if (receivedMode != RaceMode::MANUAL &&
        receivedMode != RaceMode::AUTO)
    {
        return false;
    }

    eventSequence = receivedEventSequence;

    raceNumber[0] = receivedRaceNumber[0];
    raceNumber[1] = receivedRaceNumber[1];
    raceNumber[2] = receivedRaceNumber[2];
    raceNumber[3] = receivedRaceNumber[3];
    raceNumber[4] = '\0';

    mode = receivedMode;

    return true;
}