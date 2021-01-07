#pragma once

#include <stdint.h>

class Servo {
public:
    Servo();
    uint8_t attach(int pin);
    void detach();
    void write(int value);
    void writeMicroseconds(int value);
private:
    uint8_t pin;
};
