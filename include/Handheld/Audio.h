
#pragma once

#include <stdint.h>

class AudioClass
{
public:
    void begin();
    void click(uint8_t volume);
    void boot();
    void alarm(uint8_t volume);
};

extern AudioClass Audio;
