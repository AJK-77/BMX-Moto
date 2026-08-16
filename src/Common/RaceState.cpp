#include "Common/RaceState.h"


RaceState::RaceState()
{
    reset();
}


void RaceState::reset()
{
    eventSequence = 0;
    raceNumber = 0;
}


uint16_t RaceState::getEventSequence() const
{
    return eventSequence;
}


uint16_t RaceState::getRaceNumber() const
{
    return raceNumber;
}


void RaceState::setRaceNumber(uint16_t value)
{
    if (value > 9999)
    {
        value = 9999;
    }

    raceNumber = value;
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