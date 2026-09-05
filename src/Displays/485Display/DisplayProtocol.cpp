#include "Displays/485Display/DisplayProtocol.h"

namespace DisplayProtocol
{

void buildDisplayFrame(
    const char* raceNumber,
    uint8_t* frame
)
{
    if (raceNumber == nullptr || frame == nullptr)
        return;

    // RaceState is altijd exact 4 tekens:
    // [MANCHE][MOTO honderden][MOTO tientallen][MOTO eenheden]

    const uint8_t manche =
        (raceNumber[0] >= '0' && raceNumber[0] <= '9')
        ? static_cast<uint8_t>(raceNumber[0] - '0')
        : 0;

    const uint8_t tens =
        (raceNumber[2] >= '0' && raceNumber[2] <= '9')
        ? static_cast<uint8_t>(raceNumber[2] - '0')
        : 0;

    const uint8_t ones =
        (raceNumber[3] >= '0' && raceNumber[3] <= '9')
        ? static_cast<uint8_t>(raceNumber[3] - '0')
        : 0;

    const uint8_t mancheByte =
        MANCHE_BASE + manche;

    const uint8_t tensByte =
        '0' + tens;

    const uint8_t onesByte =
        '0' + ones;

    const uint8_t checksum =
        onesByte ^ CHECKSUM_XOR;

    frame[0]  = HEADER_1;
    frame[1]  = HEADER_2;
    frame[2]  = MESSAGE_TYPE;
    frame[3]  = 0x02;
    frame[4]  = DISPLAY_DATA;
    frame[5]  = mancheByte;
    frame[6]  = SEPARATOR;
    frame[7]  = tensByte;
    frame[8]  = onesByte;
    frame[9]  = DATA_END;
    frame[10] = checksum;
    frame[11] = FRAME_END_1;
    frame[12] = FRAME_END_2;
}

}