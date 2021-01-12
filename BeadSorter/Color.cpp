#include "Color.h"
#include "Arduino.h"
#include "Ssprintf.h"

const char* ColorToString(const Colors& color)
{
    static const char* colorString[] = {
        FOREACH_COLOR(GENERATE_STRING)
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

static bool AllUnder(const Color& c, uint16_t tresh)
{
    if (c.red > tresh)
    {
        return false;
    }
    if (c.blue > tresh)
    {
        return false;
    }
    if (c.white > tresh)
    {
        return false;
    }
    if (c.green > tresh)
    {
        return false;
    }
    return true;
}

static bool AllAbove(const Color& c, uint16_t tresh)
{
    if (c.red < tresh)
    {
        return false;
    }
    if (c.blue < tresh)
    {
        return false;
    }
    if (c.white < tresh)
    {
        return false;
    }
    if (c.green < tresh)
    {
        return false;
    }
    return true;
}

Colors TcsGetColor(const Color color)
{
    ssprintf("{%s, {%u, %u, %u, %u}},", "Color", color.red, color.blue, color.white, color.green);

    if (AllUnder(color, 2500))
    {
        return Colors::BLACK;
    }
    if (AllUnder(color, 2800))
    {
        return Colors::BROWN;
    }
    if (AllAbove(color, 3400))
    {
        return Colors::GLOW;
    }
    if (AllAbove(color, 3300) && color.white < color.blue)
    {
        return Colors::WHITE;
    }
    if (color.green > color.blue && color.green > color.red)
    {
        Serial.println("Green is biggest");
        if (color.green > color.red && color.green - color.red > 120)
        {
            return Colors::GREEN;
        }
        return Colors::PALE_GREEN;
    }
    if (color.blue > color.green && color.blue > color.red)
    {
        Serial.println("Blue is biggest");
        if (color.white < color.red)
        {
            return Colors::WHITE;
        }
        if (color.white > color.red && color.white - color.red < 115)
        {
            return Colors::PURPLE;
        }
        if (color.white > color.red && color.white - color.red < 219)
        {
            return Colors::PALE_BLUE;
        }
        return Colors::BLUE;
    }
    if (color.white < color.red && color.white < color.blue && color.white < color.green)
    {
        Serial.println("White is smallest");
        if (color.red > color.white)
        {
            uint16_t rmw = color.red - color.white;
            if (rmw > 350)
            {
                if (color.green > color.white && color.green - color.white < 100)
                {
                    return Colors::ORANGE;
                }
                if (color.red > color.white && color.red - color.white < 230)
                {
                    return Colors::PALE_YELLOW;
                }
                return Colors::YELLOW;
            }
            if (rmw > 100)
            {
                if (color.green > color.white && color.green - color.white > 100)
                {
                    if (color.red > color.white && color.red - color.white < 230)
                    {
                        return Colors::PALE_YELLOW;
                    }
                    return Colors::YELLOW;
                }
                return Colors::ORANGE;
            }
            else
            {
                return Colors::GREY;
            }
        }
        return Colors::DUMP;
    }
    if (color.red > color.green && color.red > color.blue)
    {
        Serial.println("Red is biggest");
        if (color.red > color.green && color.red - color.green > 100)
        {
            if (color.red > color.white)
            {
                uint16_t rmw = color.red - color.white;
                if (rmw > 200)
                {
                    return Colors::RED;
                }
                else
                {
                    return Colors::PINK;
                }
            }
        }
        if (color.white > color.red && color.white - color.red > 20)
        {
            Serial.println("BACKUP PURPLE");
            return Colors::PURPLE;
        }
        Serial.println("BACKUP BROWN");
        return Colors::BROWN;
    }
    Serial.println("FALLOUT");
    return Colors::DUMP;
}
