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

Colors TcsGetColor()
{
    Color color = TcsReadColor();

    ssprintf("%04u, %04u, %04u, %04u", color.red, color.blue, color.white, color.green);

    for (int i = 0; i < Colors::DUMP; i++)
    {
        ssprintf("%04u, %04u, %04u, %04u", colors[i].red, colors[i].blue, colors[i].white, colors[i].green);
        if (color == colors[i])
        {
            return i;
        }
    }
    return Colors::DUMP;
}

static uint16_t readFreq()
{
    // return pulseIn(sensorOut, LOW, 250000);
    pulseIn(sensorOut, LOW, 250000);
    uint32_t time = micros();

    for (int i = 0; i < 50; i++)
    {
        if (pulseIn(sensorOut, HIGH, 25000) == 0)
        {
            Serial.println("ERROR");
        }
    }
    time = micros() - time;
    return time;
}

struct Color TcsReadColor()
{
    union ColorUnion c = {0};

    digitalWrite(LED_PIN, HIGH);

    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(S2, i & 0x01);
        digitalWrite(S3, i & 0x02);
        delay(150);

        // Reading the output frequency
        c.raw[i] = readFreq();

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
