#include "Displays/485Display/DisplayProtocol.h"

namespace DisplayProtocol
{

void buildDisplayFrame(
    uint16_t raceNumber,
    uint8_t* frame
)
{
    const uint8_t manche =
        raceNumber / 1000;

    const uint8_t tens =
        (raceNumber / 10) % 10;

    const uint8_t ones =
        raceNumber % 10;

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