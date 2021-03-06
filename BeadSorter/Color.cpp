#include "Color.h"
#include "Arduino.h"

const char* ColorToString(const Colors& color)
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
        "BROWN",
        "DUMP",

        "PALE_GREEN",
        "PALE_YELLOW",
        "PALE_SKIN",
        "PALE_BLUE",
        "PALE_PURPLE"
    };

    if (color <= Colors::PALE_PURPLE)
    {
        return (colorString[color]);
    }
    return "UNKNOWN";
}

static bool check(uint16_t l, uint16_t r, uint16_t d1, uint16_t d2)
{
    const uint16_t d = max(d1, d2);

    return (max(l, r) - min(l, r)) > d;
}

bool operator == (const Color &lhs, const Color &rhs)
{
    if (check(lhs.red, rhs.red, lhs.redDelta, rhs.redDelta))
    {
        return false;
    }
    if (check(lhs.blue, rhs.blue, lhs.blueDelta, rhs.blueDelta))
    {
        return false;
    }
    if (check(lhs.white, rhs.white, lhs.whiteDelta, rhs.whiteDelta))
    {
        return false;
    }
    if (check(lhs.green, rhs.green, lhs.greenDelta, rhs.greenDelta))
    {
        return false;
    }
    return true;
}
