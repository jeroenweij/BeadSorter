#include <unistd.h>
#include <iostream>
#include <map>

#include "Arduino.h"
#include "mock.h"

std::map<uint8_t, PinStatus> pins;

PinStatus& GetPinStatus(uint8_t pinNum)
{
    const auto& pin = pins.find(pinNum);

    if (pin != pins.end())
    {
        return pin->second;
    }
    const auto& emp = pins.emplace(std::make_pair(pinNum, PinStatus()));

    if (!emp.second)
    {
        std::cout << "Map insert failed" << std::endl;
        exit(1);
    }
    return emp.first->second;
}

void pinMode(uint8_t pinNum, uint8_t mode)
{
    GetPinStatus(pinNum).mode = static_cast<PinMode>(mode);
}

void digitalWrite(uint8_t pin, uint16_t val)
{
    GetPinStatus(pin).value = val;
}

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
    //usleep(ms * 500); // half of normal for faster execution
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
