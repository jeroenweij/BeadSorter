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

const uint32_t cMax[4]       = {1402956, 472660, 1143452, 1604424};
const uint32_t cMin[4]       = {3200, 3200, 3200, 3200};
static const uint32_t mapMax = 3500;
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
            // ssprintf("Adjusting upper bound %d from %lu to %lu", i, cMax[i], f);
            // Max[i] = f;
            f = cMax[i];
        }
        c.raw[i] = mapMax - map(f, cMin[i], cMax[i], 0, mapMax);

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
