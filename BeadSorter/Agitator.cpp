#include "Arduino.h"
#include "Agitator.h"
#include "pins.h"
#include <Servo.h>

static const uint8_t agitateUp   = 40;
static const uint8_t agitateDown = 130;

Servo agitatorServo;

void AgitatorInit()
{
    pinMode(PIN_AGITATOR_VIBRATOR, OUTPUT);
    agitatorServo.attach(PIN_AGITATOR_SERVO);
    AgitatorDown();
    // agitatorServo.detach();
}

void AgitatorUp()
{
    digitalWrite(PIN_AGITATOR_VIBRATOR, HIGH);
    agitatorServo.write(agitateUp);
}

void AgitatorDown()
{
    agitatorServo.write(agitateDown);
    digitalWrite(PIN_AGITATOR_VIBRATOR, LOW);
}

void Agitate()
{
    AgitatorUp();
    delay(500);
    AgitatorDown();
    delay(500);
}
