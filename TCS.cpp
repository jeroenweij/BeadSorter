// TCS230 or TCS3200 pins wiring to Arduino
#define LED_PIN   3
#define S0        4
#define S1        5
#define S2        6
#define S3        7
#define sensorOut 8

#include <assert.h>
#include "TCS.h"
#include "Arduino.h"

#define PRINT_BUF 100

static void ssprintf(char* fmt, ...)
{
    static char buf[PRINT_BUF];
    va_list va;

    va_start(va, fmt);
    vsnprintf(buf, PRINT_BUF, fmt, va);
    va_end(va);

    Serial.println(buf);
}

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
    if (color.green > color.blue && color.green > color.red)
    {
        return Colors::GREEN;
    }
    if (color.blue > color.green && color.blue > color.red)
    {
        if (color.red > 550)
        {
            return Colors::PURPLE;
        }
        else
        {
            return Colors::BLUE;
        }
    }
    if (color.white < color.red && color.white < color.blue && color.white < color.green)
    {
        if (color.white < 450)
        {
            return Colors::YELLOW;
        }
        if (color.red > 780)
        {
            return Colors::ORANGE;
        }
        return Colors::GREY;
    }
    if (color.green < 400)
    {
        return Colors::RED;
    }
    return Colors::PINK;
}

static uint32_t readFreq()
{
    pulseIn(sensorOut, LOW);

    uint32_t time = micros();

    for (int i = 0; i < 250; i++)
    {
        pulseIn(sensorOut, HIGH);
    }
    time = micros() - time;
    return time;
}

uint32_t cMax[4] = {578736, 215144, 744488, 744660};
uint32_t cMin[4] = {7716, 5692, 8436, 9816};
struct Color TcsReadColor()
{
    union ColorUnion c = {0};

    digitalWrite(LED_PIN, HIGH);
    delay(500);

    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(S2, i & 0x01);
        digitalWrite(S3, i & 0x02);
        delay(200);

        // Reading the output frequency
        uint32_t f = readFreq();
        if (f < cMin[i])
        {
            ssprintf("Adjusting lower bound %d from %lu to %lu", i, cMin[i], f);
            cMin[i] = f;
            ssprintf("Adjusting %d set to %lu", i, cMin[i]);

        }
        if (f > cMax[i])
        {
            ssprintf("Adjusting upper bound %d from %lu to %lu", i, cMin[i], f);
            cMax[i] = f;
            ssprintf("Adjusting %d set to %lu", i, cMax[i]);
        }
        c.raw[i] = 1000 - map(f, cMin[i], cMax[i], 0, 1000);

        delay(50);
    }

    digitalWrite(LED_PIN, LOW);

    return c.color;
}

void TcsInit()
{
    // Setting the outputs
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    // Setting frequency scaling to 20%
    digitalWrite(LED_PIN, LOW);
    // Setting the sensorOut as an input
    pinMode(sensorOut, INPUT);

    // Setting frequency scaling to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, 0);
}
