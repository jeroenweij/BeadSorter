#include "Arduino.h"
#include <unistd.h>
#include "mock.h"

void pinMode(uint8_t, uint8_t)
{ }

void digitalWrite(uint8_t pin, uint8_t val)
{ }

int16_t digitalRead(uint8_t pin)
{
    RETURN_MOCK;
}

int16_t analogRead(uint8_t pin)
{
    RETURN_MOCK;
}

void delay(uint32_t ms)
{
    usleep(ms * 500); // half of normal for faster execution
}

uint32_t pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    RETURN_MOCK;
}

uint32_t micros()
{
    RETURN_MOCK;
}

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
