#pragma once

class USBService
{
public:
    USBService();

    void begin();
    void update();

private:
    void processCommand(const char* command);
};