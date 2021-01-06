#include "Arduino.h"
#include "Agitator.h"
#include <Servo.h>

static const uint8_t agitateUp   = 40;
static const uint8_t agitateDown = 130;

#define AGITATOR_PIN 14
#define VIBRATION_PIN 15

Servo agitatorServo;

void AgitatorInit()
{
    pinMode(VIBRATION_PIN, OUTPUT);
    agitatorServo.attach(AGITATOR_PIN);
    AgitatorDown();
    // agitatorServo.detach();
}

void AgitatorUp()
{
    agitatorServo.write(agitateUp);
    digitalWrite(VIBRATION_PIN, HIGH);
}

void AgitatorDown()
{
    agitatorServo.write(agitateDown);
    digitalWrite(VIBRATION_PIN, LOW);
}

void Agitate()
{
    AgitatorUp();
    delay(500);
    AgitatorDown();
    delay(500);
}
