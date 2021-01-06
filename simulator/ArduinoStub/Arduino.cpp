#include "Arduino.h"
#include <unistd.h>

void pinMode(uint8_t, uint8_t)
{ }

void digitalWrite(uint8_t pin, uint8_t val)
{ }

int16_t digitalRead(uint8_t pin)
{
    return HIGH;
}

int16_t analogRead(uint8_t pin)
{
    return 0;
}

void delay(uint32_t ms)
{
    usleep(ms * 1000);
}

uint32_t pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    return 0;
}

uint32_t micros()
{
    return 0;
}

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
