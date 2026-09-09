#include "Displays/485Display/DisplayProtocol.h"

namespace DisplayProtocol
{

void buildDisplayFrame(const char* raceNumber, uint8_t* frame)
{
    if (raceNumber == nullptr || frame == nullptr)
    {
        return;
    }

    // ========================================================
    // RaceState is ALTIJD 4 digits:
    //
    // [0] [1] [2] [3]
    //
    // Voorbeeld:
    //  1   2   3   4
    //
    // Het oude fysieke RS485-display heeft slechts
    // 3 zichtbare digits:
    //
    //  1   _   3   4
    //
    // De tweede RaceState-digit wordt dus NIET verstuurd.
    // ========================================================

    const uint8_t d0 =
        (raceNumber[0] >= '0' && raceNumber[0] <= '9')
        ? static_cast<uint8_t>(raceNumber[0] - '0')
        : 0;

    const uint8_t d1 =
        (raceNumber[1] >= '0' && raceNumber[1] <= '9')
        ? static_cast<uint8_t>(raceNumber[1] - '0')
        : 0;

    const uint8_t d2 =
        (raceNumber[2] >= '0' && raceNumber[2] <= '9')
        ? static_cast<uint8_t>(raceNumber[2] - '0')
        : 0;

    const uint8_t d3 =
        (raceNumber[3] >= '0' && raceNumber[3] <= '9')
        ? static_cast<uint8_t>(raceNumber[3] - '0')
        : 0;

    // d1 is bewust niet onderdeel van het oude 3-digit
    // RS485 displayframe.
    (void)d1;

    Serial.printf(
        "DISPLAY TRANSLATE: %c%c%c%c -> %u _ %u %u\n",
        raceNumber[0],
        raceNumber[1],
        raceNumber[2],
        raceNumber[3],
        d0,
        d2,
        d3
    );

    // ========================================================
    // Eerste fysieke digit
    //
    // 0 -> B0
    // 1 -> B1
    // ...
    // 9 -> B9
    // ========================================================

    const uint8_t firstDigitByte =
        FIRST_DIGIT_BASE + d0;

    // De twee zichtbare cijfers worden als ASCII verstuurd.
    const uint8_t digit1Byte = '0' + d2;
    const uint8_t digit2Byte = '0' + d3;

    // ========================================================
    // Bewezen checksum
    //
    // checksum =
    //     B?
    //     ^ 20
    //     ^ ASCII digit 1
    //     ^ ASCII digit 2
    //     ^ 03
    //     ^ BE
    // ========================================================

    const uint8_t checksum =
        firstDigitByte ^
        SEPARATOR ^
        digit1Byte ^
        digit2Byte ^
        DATA_END ^
        CHECKSUM_XOR;

    // ========================================================
    // Frame
    //
    // 04 04 81 02 3D B? 20 XX XX 03 CC 06 06
    // ========================================================

    frame[0]  = HEADER_1;
    frame[1]  = HEADER_2;
    frame[2]  = MESSAGE_TYPE;
    frame[3]  = 0x02;
    frame[4]  = DISPLAY_DATA;
    frame[5]  = firstDigitByte;
    frame[6]  = SEPARATOR;
    frame[7]  = digit1Byte;
    frame[8]  = digit2Byte;
    frame[9]  = DATA_END;
    frame[10] = checksum;
    frame[11] = FRAME_END_1;
    frame[12] = FRAME_END_2;

    // ========================================================
    // Debug
    // ========================================================

    Serial.print("DISPLAY FRAME: ");

    for (size_t i = 0; i < DISPLAY_FRAME_SIZE; i++)
    {
        if (frame[i] < 0x10)
        {
            Serial.print('0');
        }

        Serial.print(frame[i], HEX);
        Serial.print(' ');
    }

    Serial.println();
}

}