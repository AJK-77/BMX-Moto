#pragma once

#include <stdint.h>

class AudioClass
{
public:
    void begin();
    void click(uint8_t volume);
    void boot();
};

extern AudioClass Audio;