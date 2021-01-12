#include "Arduino.h"
#include "Dropper.h"
#include "pins.h"
#include "Ssprintf.h"
#include <Servo.h>

const uint8_t positions[] = {14, // 1
                             30,
                             46, // 3
                             63,
                             81, // 5
                             98,
                             115, // 7
                             132,
                             148, // 9
                             165, };

static const uint8_t numSpaces   = sizeof(positions);
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
    return color;
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
    int mic = positions[pos];

//    if (pos < oldPos)
//    {
//        mic -= 1;
//    }
    dropperServo.attach(PIN_DROPPER_SERVO);
    dropperServo.write(mic);

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
        // tubbleServo.detach();
    }
    // dropperServo.detach();
}
