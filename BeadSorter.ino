#include "TCS.h"

#define defdelta  10
#define PRINT_BUF 100

static void ssprintf(char* fmt, ...)
{
    static char buf[PRINT_BUF];
    va_list va;
    va_start(va, fmt);
    vsnprintf(buf, PRINT_BUF, fmt, va);
    va_end(va);

    Serial.print(buf);
}

static const Color colors[] =
{
    // R B  W  G  D
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
    ssprintf("%04d, %04d, %04d, %04d", c.red, c.blue, c.white, c.green);
    Serial.println();
    delay(250);
}
