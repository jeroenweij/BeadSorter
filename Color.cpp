
#include "Color.h"
#include "Arduino.h"

void ColorPrintName(Colors& color)
{
    static const char* colorString[] =
    {
        "NONE",
        "RED",
        "GREEN",
        "BLUE",
        "YELLOW",
        "ORANGE",
        "PINK",
        "PURPLE",
        "GREY",
        "BLACK",
        "WHITE",
        "GLOW",
        "PALE_GREEN",
        "PALE_YELLOW",
        "DUMP",
    };

    Serial.print("The color is: ");
    Serial.println(colorString[static_cast < uint16_t > (color)]);
}

bool operator == (const Color &lhs, const Color &rhs)
{
    const uint16_t delta = max(lhs.delta, rhs.delta);

    if (abs(lhs.red - rhs.red) > delta)
    {
        return false;
    }
    if (abs(lhs.blue - rhs.blue) > delta)
    {
        return false;
    }
    if (abs(lhs.white - rhs.white) > delta)
    {
        return false;
    }
    if (abs(lhs.green - rhs.green) > delta)
    {
        return false;
    }
    return true;
}
