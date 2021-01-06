#include "Arduino.h"
#include "Dropper.h"
#include "pins.h"
#include <Servo.h>

static const uint16_t space      = 240;
static const uint16_t turnOffset = 570;
static const uint8_t numSpaces   = 7;
static const uint8_t tumbleLeft  = 40;
static const uint8_t tumbleRigth = 130;

static bool tumbleIsRigth = true;
static int8_t oldPos      = 0;

Servo dropperServo;
Servo tubbleServo;

static bool setTubler(bool rigth)
{
    Serial.print("Tumble to ");
    Serial.println(rigth ? "RIGTH" : "LEFT");
    bool change = rigth != tumbleIsRigth;

    if (change)
    {
        Serial.println("TUMBLING");
        tubbleServo.attach(PIN_TUMBLER_SERVO);
        tubbleServo.write(rigth ? tumbleRigth : tumbleLeft);
    }
    tumbleIsRigth = rigth;
    return change;
}

void DropperInit()
{
    DropperSetPos(0);
}

void DropperSetPos(int8_t pos)
{
    if (pos > 13)
    {
        return;
    }

    // in steps of 1 degree
    bool tChange = setTubler(pos - numSpaces >= 0);

    pos %= numSpaces;

    Serial.print("pos to ");
    Serial.println(pos + 1);
    int mic = (pos * space) + turnOffset;

    // Serial.print("mic to ");
    // Serial.println(mic);
    if (pos < oldPos)
    {
        mic -= 25;
    }
    dropperServo.attach(PIN_DROPPER_SERVO);
    dropperServo.writeMicroseconds(mic);

    int8_t offset = abs(pos - oldPos);
    int calcDelay = 0;

    if (offset > 0)
    {
        calcDelay = ((offset * 130) + 195);
        // Serial.print("calcDelay to ");
        // Serial.println(calcDelay);
    }
    if (tChange)
    {
        calcDelay = max(800, calcDelay);
    }
    oldPos = pos;
    // Serial.print("calcDelay to ");
    // Serial.println(calcDelay);
    delay(calcDelay);
    if (tChange)
    {
        tubbleServo.detach();
    }
    dropperServo.detach();
}
