#include "TCS.h"
#include "Dropper.h"
#include "Agitator.h"
#include "pins.h"
#include "Ssprintf.h"

#define TEST_I 15
static void testColor()
{
    struct Color c    = {0};
    struct Color cmin = {0};
    struct Color cmax = {0};

    TcsReadColor();
    delay(2000);

    ssprintf("X, Red, Blue, White, Green");
    for (uint16_t i = 0; i < TEST_I; i++)
    {
        c = TcsReadColor();
        if (i == 0)
        {
            cmin = c;
            cmax = c;
        }
        else
        {
            cmin.red   = min(cmin.red, c.red);
            cmin.blue  = min(cmin.blue, c.blue);
            cmin.white = min(cmin.white, c.white);
            cmin.green = min(cmin.green, c.green);

            cmax.red   = max(cmax.red, c.red);
            cmax.blue  = max(cmax.blue, c.blue);
            cmax.white = max(cmax.white, c.white);
            cmax.green = max(cmax.green, c.green);
        }
        ssprintf("%03u, %u, %u, %u, %u", i, c.red, c.blue, c.white, c.green);
        delay(2000);
    }

    ssprintf("MIN, %u, %u, %u, %u", cmin.red, cmin.blue, cmin.white, cmin.green);
    ssprintf("MAX, %u, %u, %u, %u", cmax.red, cmax.blue, cmax.white, cmax.green);
    //    Serial.println("DIFF");
    //    ssprintf("%u, %u, %u, %u", cmax.red - cmin.red, cmax.blue - cmin.blue,
    //      cmax.white - cmin.white, cmax.green - cmin.green);

    Color cdelta;

    cdelta.redDelta   = (cmax.red - cmin.red) / 2;
    cdelta.blueDelta  = (cmax.blue - cmin.blue) / 2;
    cdelta.whiteDelta = (cmax.white - cmin.white) / 2;
    cdelta.greenDelta = (cmax.green - cmin.green) / 2;

    uint32_t add;

    add          = (uint32_t) cmax.red + (uint32_t) cmin.red;
    cdelta.red   = add / 2u;
    add          = (uint32_t) cmax.blue + (uint32_t) cmin.blue;
    cdelta.blue  = add / 2u;
    add          = (uint32_t) cmax.white + (uint32_t) cmin.white;
    cdelta.white = add / 2u;
    add          = (uint32_t) cmax.green + (uint32_t) cmin.green;
    cdelta.green = add / 2u;

    ssprintf("AVR, %u, %u, %u, %u, %u, %u, %u, %u", cdelta.red, cdelta.blue, cdelta.white, cdelta.green,
      cdelta.redDelta, cdelta.blueDelta, cdelta.whiteDelta, cdelta.greenDelta);
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
    Serial.println("20: ");
    Serial.println("Test all colors seperated by black beads");
    Serial.println("-------------------------");
    Serial.println("21: ");
    Serial.println("Empty the queue");
    Serial.println("-------------------------");
}

static bool BeadAvailable()
{
    digitalWrite(PIN_AVAILABILITY_LED, HIGH);
    delay(50);
    int v = analogRead(PIN_AVAILABILITY_SENSOR);

    ssprintf("Availability sensor reads: %d", v);
    digitalWrite(PIN_AVAILABILITY_LED, LOW);
    return true; // TODO v <> X
}

static void DropBead()
{
    Serial.println("Dropping");
    digitalWrite(PIN_SOLENOID, HIGH);
    delay(400);
    digitalWrite(PIN_SOLENOID, LOW);
    delay(600);
}

static void ClearQueue()
{
    Serial.println("CLEARIN QUEUE");
    DropperSetPos(Colors::DUMP);
    //    while (BeadAvailable())
    //    {
    //        DropBead();
    //        delay(100);
    //    }
    for (int i = 0; i < 10; i++)
    {
        DropBead();
        delay(100);
    }
    Serial.println("Queue cleared");
}

void setup()
{
    // Begins serial communication
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_SOLENOID, OUTPUT);
    pinMode(PIN_AVAILABILITY_SENSOR, INPUT);
    pinMode(PIN_AVAILABILITY_LED, OUTPUT);
    digitalWrite(PIN_SOLENOID, LOW);
    digitalWrite(PIN_AVAILABILITY_LED, LOW);

    // ClearQueue();
    TcsInit();
    DropperInit();
    AgitatorInit();

    delay(100);

    printHelp();
    Serial.println("READY!");
}

static void testAll()
{
    Colors toTest[] =
    {
        RED,
        GREEN,
        BLUE,
        YELLOW,
        ORANGE,
        PINK,
        PURPLE,
        GREY,
        BLACK,
        WHITE,
        GLOW,
        BROWN,
        PALE_GREEN,
    };

    for (const auto &c : toTest)
    {
        ssprintf("Testing color: %s ", ColorToString(c));

        testColor();
        DropBead();
        DropBead();
        delay(5000);
    }
}

static Colors TwoOutOFThree()
{
    while (true)
    {
        Colors c1 = TcsGetColor();
        Colors c2 = TcsGetColor();
        if (c1 == c2)
        {
            return c1;
        }
        Colors c3 = TcsGetColor();
        if (c1 == c3)
        {
            return c1;
        }
        if (c2 == c3)
        {
            return c2;
        }
    }
}

static void SortOne()
{
    Colors c = TwoOutOFThree();
    static Colors prevC = Colors::DUMP;

    ssprintf("Detected color: %s ", ColorToString(c));

    if (c != Colors::NONE)
    {
        ssprintf("Dropping color: %s ", ColorToString(prevC));
        AgitatorUp();
        DropperSetPos(prevC);
        DropBead();
        prevC = c;
        AgitatorDown();
    }
}

void loop()
{
    delay(100);
    if (digitalRead(PIN_BUTTON) == LOW)
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
                DropperSetPos(static_cast < Colors > (state));
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
                ssprintf("> Bead is %s available. (%d)", BeadAvailable() ? "wel" : "NOT", state);
                break;
            case 20:
                ssprintf("> Testing all colors (%d)", state);
                testAll();
                break;
            case 21:
                ssprintf("> Emptying the queue (%d)", state);
                ClearQueue();
                break;
            default:
                ssprintf("> cannot execute command (%d)", state);
                printHelp();
                break;
        }
    }
}
