#include "Common/RaceState.h"


RaceState::RaceState()
{
    reset();
}


void RaceState::reset()
{
    eventSequence = 0;
    raceNumber = 0;
    mode = RaceMode::MANUAL;
}


uint16_t RaceState::getEventSequence() const
{
    return eventSequence;
}


uint16_t RaceState::getRaceNumber() const
{
    return raceNumber;
}


RaceMode RaceState::getMode() const
{
    return mode;
}


void RaceState::setRaceNumber(uint16_t value)
{
    if (value > 9999)
    {
        value = 9999;
    }

    raceNumber = value;
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

    uint16_t manche = raceNumber / 1000;
    uint16_t moto = raceNumber % 1000;

    moto++;

    if (moto > 999)
    {
        moto = 0;
    }

    raceNumber = (manche * 1000) + moto;
}


void RaceState::setEventSequence(uint16_t value)
{
    eventSequence = value;

    if (eventSequence == 0)
    {
        mode = RaceMode::MANUAL;
    }
}


bool RaceState::applyState(uint16_t receivedEventSequence,
                           uint16_t receivedRaceNumber,
                           RaceMode receivedMode)
{
    if (receivedEventSequence <= eventSequence)
    {
        return false;
    }

    if (receivedRaceNumber > 9999)
    {
        return false;
    }

    if (receivedMode != RaceMode::MANUAL &&
        receivedMode != RaceMode::AUTO)
    {
        return false;
    }

    eventSequence = receivedEventSequence;
    raceNumber = receivedRaceNumber;
    mode = receivedMode;

    return true;
}