#include "TCS.h"
#include "Dropper.h"

#define PRINT_BUF  100
#define LED_PIN    3
#define BUTTON_PIN 2

static void ssprintf(char* fmt, ...)
{
    static char buf[PRINT_BUF];
    va_list va;

    va_start(va, fmt);
    vsnprintf(buf, PRINT_BUF, fmt, va);
    va_end(va);

    Serial.println(buf);
}

#define TEST_I 10
static void testColor()
{
    struct Color c[TEST_I] = {0};
    struct Color cmin      = {0};
    struct Color cmax      = {0};

    for (uint16_t i = 0; i < TEST_I; i++)
    {
        c[i] = TcsReadColor();
        if (i == 0)
        {
            cmin = c[i];
            cmax = c[i];
        }
        else
        {
            cmin.red   = min(cmin.red, c[i].red);
            cmin.blue  = min(cmin.blue, c[i].blue);
            cmin.white = min(cmin.white, c[i].white);
            cmin.green = min(cmin.green, c[i].green);

            cmax.red   = max(cmax.red, c[i].red);
            cmax.blue  = max(cmax.blue, c[i].blue);
            cmax.white = max(cmax.white, c[i].white);
            cmax.green = max(cmax.green, c[i].green);
        }
    }

    Serial.println("MIN");
    ssprintf("%04u, %04u, %04u, %04u", cmin.red, cmin.blue, cmin.white, cmin.green);
    Serial.println("MAX");
    ssprintf("%04u, %04u, %04u, %04u", cmax.red, cmax.blue, cmax.white, cmax.green);
    Serial.println("DIFF");
    ssprintf("%04u, %04u, %04u, %04u", cmax.red - cmin.red, cmax.blue - cmin.blue,
      cmax.white - cmin.white, cmax.green - cmin.green);
    Serial.println();
}

void setup()
{
    // Begins serial communication
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    Serial.println("-------------------------");
    Serial.println("Comand input online, write command to perform action");
    Serial.println("-------------------------");
    Serial.print("unsigned long ");
    Serial.println(sizeof(unsigned long));
    Serial.print("unsigned int ");
    Serial.println(sizeof(unsigned int));

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    TcsInit();
    DropperInit();

    delay(1000);
}

static void SortOne()
{
    Colors c = TcsGetColor();

    ColorPrintName(c);

    if (c != Colors::NONE)
    {
        DropperSetPos(static_cast < int8_t > (c) - 1);
        // TODO: drop the bead!

        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
}

void loop()
{
    delay(100);
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        SortOne();
    }

    while (Serial.available())
    {
        int state = Serial.parseInt();

        switch (state)
        {
            case 0:
                break;
            case 1 ... 14:
                ssprintf("> turning to (%d)", state);
                DropperSetPos(state - 1);
                break;
            case 15:
                ssprintf("> Testing color (%d)", state);
                testColor();
                break;
            case 16:
                ssprintf("> Sort 1 bead (%d)", state);
                SortOne();
                break;
            default:
                ssprintf("> cannot execute command (%d)", state);
                break;
        }
    }
}
