#include "Handheld/GUI/GUI.h"

GUI::GUI()
{
}

void GUI::begin()
{
    if (initialized)
        return;

    initialized = true;

    Serial.println("GUI: initialized");
}

void GUI::update()
{
    if (!initialized)
        return;

    // GUI update komt hier.
}