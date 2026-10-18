#include "Handheld/Audio.h"
#include <Arduino.h>
#include <math.h>

#define AUDIO_OUT 26

AudioClass Audio;

void AudioClass::begin()
{
    // DAC niet actief houden:
    dacWrite(AUDIO_OUT, 0);
}

void AudioClass::boot()
{
    // lage toon
    for (int i = 0; i < 200; i++)
    {
        dacWrite(AUDIO_OUT, 200);
        delayMicroseconds(500);

        dacWrite(AUDIO_OUT, 56);
        delayMicroseconds(500);
    }

    dacWrite(AUDIO_OUT, 0);
    delay(50);

    // hogere toon
    for (int i = 0; i < 300; i++)
    {
        dacWrite(AUDIO_OUT, 210);
        delayMicroseconds(400);

        dacWrite(AUDIO_OUT, 46);
        delayMicroseconds(400);
    }

    dacWrite(AUDIO_OUT, 0);
}

void AudioClass::click(uint8_t volume)
{
    if (volume == 0)
    {
        dacWrite(AUDIO_OUT, 0);
        return;
    }

    uint8_t high = map(volume, 1, 100, 180, 255);
    uint8_t low  = map(volume, 1, 100, 75, 0);

    for (int i = 0; i < 25; i++)
    {
        dacWrite(AUDIO_OUT, high);
        delayMicroseconds(200);

        dacWrite(AUDIO_OUT, low);
        delayMicroseconds(200);
    }

    dacWrite(AUDIO_OUT, 0);
}