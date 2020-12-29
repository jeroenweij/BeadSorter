// TCS230 or TCS3200 pins wiring to Arduino
#define S0        4
#define S1        5
#define S2        6
#define S3        7
#define sensorOut 8

#include <assert.h>
#include "TCS.h"
#include "Arduino.h"

union ColorUnion
{
    struct Color color;
    uint16_t     raw[5];
    static_assert(sizeof(raw) == sizeof(struct Color), "Sizes must match");
};

struct Color TcsGetColor()
{
    union ColorUnion c;

    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(S2, i & 0x01);
        digitalWrite(S3, i & 0x02);
        delay(50);

        // Reading the output frequency
        c.raw[i] = pulseIn(sensorOut, LOW);

        delay(50);
    }

    return c.color;
}

void TcsInit()
{
    // Setting the outputs
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    // Setting the sensorOut as an input
    pinMode(sensorOut, INPUT);

    // Setting frequency scaling to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, 0);
}

bool operator ==(const Color &lhs, const Color &rhs)
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
