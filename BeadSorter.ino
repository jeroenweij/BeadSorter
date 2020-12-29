#include "TCS.h"

#define defdelta 10

static const Color colors[] =
{
    //R B  W  G  D
    {0, 0, 0, 0, defdelta},
    {0, 0, 0, 0, defdelta}
};

void setup()
{
    TcsInit();

    // Begins serial communication
    Serial.begin(9600);
}

void loop()
{
    struct Color c  = TcsGetColor();
    struct Color c2 = TcsGetColor();

    c2.delta = 10;

    if (c == c2)
    {
        Serial.println("MATCH");
    }

    Serial.print(" R = ");
    Serial.print(c.red);
    Serial.print(" G = ");
    Serial.print(c.green);
    Serial.print(" B = ");
    Serial.print(c.blue);
    Serial.print(" W = ");
    Serial.println(c.white);
    delay(250);
}
