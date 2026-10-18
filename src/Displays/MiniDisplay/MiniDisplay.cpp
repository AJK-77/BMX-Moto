#include "Displays/MiniDisplay/MiniDisplay.h"
#include "Displays/MiniDisplay/MiniTFT.h"
#include "Displays/MiniDisplay/RapidWheelsLogo.h"

MiniDisplay miniDisplay;

bool MiniDisplay::begin()
{
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);

    MiniTFT.begin();
    MiniTFT.showSplash();
    //draw();

    return true;
}

void MiniDisplay::update()
{
    static bool splashDone = false;

    if (!splashDone)
    {
        if (MiniTFT.splashFinished())
        {
            splashDone = true;
            draw();
        }

        return;
    }
}

void MiniDisplay::draw()
{
    MiniTFT.clear();

    // Status
   MiniTFT.drawText(5, 5, statusText, 1);

    // Labels
    MiniTFT.drawText(10, 30, "MOTO", 3);
    MiniTFT.drawText(200, 30, "RACE", 3);

    const char *raceNumber = raceState->getRaceNumber();

    char mancheText[2];
    char raceText[4];

    mancheText[0] = raceNumber[0];
    mancheText[1] = '\0';

    raceText[0] = raceNumber[1];
    raceText[1] = raceNumber[2];
    raceText[2] = raceNumber[3];
    raceText[3] = '\0';

    // Grote waarden
    MiniTFT.drawText(30, 90, mancheText, 8);
    MiniTFT.drawText(150, 90, raceText, 8);
}

void MiniDisplay::drawManche(uint8_t manche)
{
    // Voor later
}

void MiniDisplay::drawRace(uint16_t race)
{
    // Voor later
}

void MiniDisplay::setRaceState(RaceState *state)
{
    raceState = state;
}

void MiniDisplay::onStateChanged()
{
    draw();
}

void MiniDisplay::onHeartbeatLost(uint8_t node)
{
    if (node >= 1 && node <= 10)
    {
        snprintf(
            statusText,
            sizeof(statusText),
            "LOST: Handheld %u",
            node
        );
    }
    else if (node >= 11 && node <= 20)
    {
        snprintf(
            statusText,
            sizeof(statusText),
            "LOST: Gate %u",
            node - 10
        );
    }
    else if (node >= 21 && node <= 30)
    {
        snprintf(
            statusText,
            sizeof(statusText),
            "LOST: Display %u",
            node - 20
        );
    }
    else
    {
        snprintf(
            statusText,
            sizeof(statusText),
            "LOST: Node %u",
            node
        );
    }

    draw();
}

void MiniDisplay::onHeartbeatRestored()
{
    strncpy(
        statusText,
        "COMMUNICATIE OK",
        sizeof(statusText) - 1
    );

    statusText[sizeof(statusText) - 1] = '\0';

    draw();
}
