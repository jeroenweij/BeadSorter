#include <assert.h>

#include "TCS.h"
#include "Arduino.h"
#include "pins.h"
#include "Ssprintf.h"

union ColorUnion
{
    struct Color color;
    uint16_t     raw[8];
    static_assert(sizeof(raw) == sizeof(struct Color), "Sizes must match");
};

static bool AllUnder(Color& c, uint16_t tresh)
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

static bool AllAbove(Color& c, uint16_t tresh)
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

Colors TcsGetColor()
{
    Color color = TcsReadColor();

    ssprintf("%u, %u, %u, %u", color.red, color.blue, color.white, color.green);

    if (AllUnder(color, 100))
    {
        return Colors::BLACK;
    }
    if (AllUnder(color, 350))
    {
        return Colors::BROWN;
    }
    if (AllAbove(color, 950))
    {
        return Colors::GLOW;
    }
    if (AllAbove(color, 760))
    {
        return Colors::WHITE;
    }
    if (color.green > color.blue && color.green > color.red)
    {
        Serial.println("Green is biggest");
        return Colors::GREEN;
    }
    if (color.blue > color.green && color.blue > color.red)
    {
        Serial.println("Blue is biggest");
        if (color.red > color.white)
        {
            Serial.println("Backup GREY");
            return Colors::BLUE;
        }
        if (color.white > color.red && color.white - color.red < 200)
        {
            Serial.println("Backup PURPLE");
            return Colors::PURPLE;
        }

        return Colors::BLUE;
    }
    if (color.white < color.red && color.white < color.blue && color.white < color.green)
    {
        Serial.println("White is smallest");
        if (color.blue > color.green && color.blue - color.green > 150)
        {
            return Colors::ORANGE;
        }
        if (color.red > color.white)
        {
            if (color.red > 750)
            {
                return Colors::YELLOW;
            }
            return Colors::GREY;
        }

        return Colors::DUMP;
    }
    if (color.red > color.green && color.red > color.blue)
    {
        Serial.println("Red is biggest");
        if (color.white > color.blue)
        {
            return Colors::PURPLE;
        }
        if (color.blue > color.white && color.blue - color.white > 150)
        {
            return Colors::RED;
        }
        if (color.red < 700)
        {
            return Colors::BROWN;
        }
        else
        {
            return Colors::PINK;
        }
        return Colors::RED;
    }
    Serial.println("FALLOUT");
    return Colors::DUMP;
}

static uint32_t readFreq()
{
    pulseIn(PIN_TCS_OUT, LOW);

    uint32_t time = micros();

    for (int i = 0; i < 250; i++)
    {
        pulseIn(PIN_TCS_OUT, HIGH);
    }
    time = micros() - time;
    return time;
}

const uint32_t cMax[4] = {410000, 140000, 310000, 450000};
const uint32_t cMin[4] = {3168, 3168, 3168, 3168};
const uint32_t maxOver[4] = {378620,118080,319504,433124};

struct Color TcsReadColor()
{
    union ColorUnion c = {0};

    digitalWrite(PIN_TCS_LED, HIGH);
    delay(500);

    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(PIN_TCS_S2, i & 0x01);
        digitalWrite(PIN_TCS_S3, (i & 0x02) > 0);
        delay(200);

        // Reading the output frequency
        uint32_t f = readFreq();
        if (f < cMin[i])
        {
            // ssprintf("Adjusting lower bound %d from %lu to %lu", i, cMin[i], f);
            // cMin[i] = f;
            f = cMin[i];
        }
        if (f > cMax[i])
        {
            ssprintf("Adjusting upper bound %d from %lu to %lu", i, cMax[i], f);
            // cMax[i] = f;
            uint32_t overshot = f - cMax[i];
            if (overshot > maxOver[i])
                overshot = maxOver[i];
            uint32_t correction = maxOver[i] - overshot;
            correction/=100;
            f = cMax[i] - correction;
        }
        c.raw[i] = 1000 - map(f, cMin[i], cMax[i], 0, 1000);

        delay(50);
    }

    digitalWrite(PIN_TCS_LED, LOW);

    return c.color;
}

void TcsInit()
{
    // Setting the outputs
    pinMode(PIN_TCS_S0, OUTPUT);
    pinMode(PIN_TCS_S1, OUTPUT);
    pinMode(PIN_TCS_S2, OUTPUT);
    pinMode(PIN_TCS_S3, OUTPUT);
    pinMode(PIN_TCS_LED, OUTPUT);

    // Setting frequency scaling to 20%
    digitalWrite(PIN_TCS_LED, LOW);
    // Setting the sensorOut as an input
    pinMode(PIN_TCS_OUT, INPUT);

    // Setting frequency scaling to 20%
    digitalWrite(PIN_TCS_S0, HIGH);
    digitalWrite(PIN_TCS_S1, LOW);
}
