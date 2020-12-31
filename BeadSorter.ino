#include "TCS.h"
#include "Dropper.h"
#include "Agitator.h"

#define PRINT_BUF           100
#define SOLENOID_PIN        16
#define BUTTON_PIN          2
#define AVAILABILITY_LED    18
#define AVAILABILITY_SENSOR A1

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

static void printHelp()
{
    Serial.println("-------------------------");
    Serial.println("Comand input online, command options:");
    Serial.println("-------------------------");

    Serial.println("1 - 14: ");
    Serial.println("Move dropper to position (1 - 14)");
    Serial.println("-------------------------");
    Serial.println("15: ");
    Serial.println("Test the current color");
    Serial.println("-------------------------");
    Serial.println("16: ");
    Serial.println("Sort one bead");
    Serial.println("-------------------------");
    Serial.println("17: ");
    Serial.println("Agitate");
    Serial.println("-------------------------");
    Serial.println("18: ");
    Serial.println("Drop 1 bead");
    Serial.println("-------------------------");
    Serial.println("19: ");
    Serial.println("Check if there are enough beads to proceed");
    Serial.println("-------------------------");
}

void setup()
{
    // Begins serial communication
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    printHelp();

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(SOLENOID_PIN, OUTPUT);
    pinMode(AVAILABILITY_SENSOR, INPUT);
    pinMode(AVAILABILITY_LED, OUTPUT);
    digitalWrite(SOLENOID_PIN, LOW);
    digitalWrite(AVAILABILITY_LED, LOW);

    TcsInit();
    DropperInit();
    AgitatorInit();

    delay(100);
}

static void DropBead()
{
    digitalWrite(SOLENOID_PIN, HIGH);
    delay(250);
    digitalWrite(SOLENOID_PIN, LOW);
}

static bool BeadAvailable()
{
    digitalWrite(AVAILABILITY_LED, HIGH);
    delay(50);
    int v = analogRead(AVAILABILITY_SENSOR);

    ssprintf("Availability sensor reads: %d", v);
    digitalWrite(AVAILABILITY_LED, LOW);
    return true; // TODO v <> X
}

static void SortOne()
{
    Colors c = TcsGetColor();

    ColorPrintName(c);

    if (c != Colors::NONE)
    {
        AgitatorUp();
        DropperSetPos(static_cast < int8_t > (c) - 1);
        DropBead();
        AgitatorDown();
    }
}

void loop()
{
    delay(100);
    if (digitalRead(BUTTON_PIN) == LOW)
    {
        if (BeadAvailable())
        {
            SortOne();
        }
        else
        {
            Agitate();
        }
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
            case 17:
                ssprintf("> Agitating (%d)", state);
                Agitate();
                break;
            case 18:
                ssprintf("> Dropping (%d)", state);
                DropBead();
                break;
            case 19:
                ssprintf("> Bead is %s available. (%d)", state, BeadAvailable() ? "wel" : "NOT");
                break;
            default:
                ssprintf("> cannot execute command (%d)", state);
                printHelp();
                break;
        }
    }
}
