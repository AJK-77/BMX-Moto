#pragma once

class USBManager
{
public:
    USBManager();

    void begin();
    void update();

private:
    void processCommand(const char* command);
};