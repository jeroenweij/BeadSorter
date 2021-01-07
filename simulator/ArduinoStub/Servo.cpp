#include "Servo.h"
#include "Arduino.h"

Servo::Servo()
{ }

uint8_t Servo::attach(int newPin)
{
    pin = newPin;
    pinMode(pin, SERVO);
    return 0;
}

void Servo::detach()
{ }

void Servo::write(int value)
{
    digitalWrite(pin, value);
}

void Servo::writeMicroseconds(int value)
{
    digitalWrite(pin, value / 4);
}
