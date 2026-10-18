#pragma once

#include <stdint.h>
#include "Common/RaceState.h"

class MiniDisplay
{
public:
    bool begin();
    void update();
    void draw();
    void setRaceState(RaceState* state);
    void onStateChanged();
    void onHeartbeatLost(uint8_t node);
    void onHeartbeatRestored();

private:
    void drawManche(uint8_t manche);
    void drawRace(uint16_t race);

    char statusText[32] = "COMMUNICATIE OK";

    RaceState* raceState = nullptr;
    
};

extern MiniDisplay miniDisplay;