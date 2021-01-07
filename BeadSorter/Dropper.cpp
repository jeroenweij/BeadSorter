#include "Arduino.h"
#include "Dropper.h"
#include "pins.h"
#include "Ssprintf.h"
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

static int8_t ColorToDropPos(Colors color)
{
    if (color != Colors::NONE && color <= Colors::DUMP)
    {
        return color - 1;
    }
    return 13;
}

void DropperInit()
{
    DropperSetPos(Colors::RED);
}

void DropperSetPos(Colors color)
{
    ssprintf("Dropper pos to %s ", ColorToString(color));

    int8_t pos = ColorToDropPos(color);

    // in steps of 1 degree
    bool tChange = setTubler(pos - numSpaces >= 0);

    pos %= numSpaces;

    Serial.print("pos to ");
    Serial.println(pos + 1);
    int mic = (pos * space) + turnOffset;

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
    }
    if (tChange)
    {
        calcDelay = max(800, calcDelay);
    }
    oldPos = pos;
    delay(calcDelay);
    if (tChange)
    {
        tubbleServo.detach();
    }
    dropperServo.detach();
}
