#include "TCS.h"
#include "Dropper.h"
#include "Agitator.h"
#include "pins.h"
#include "Ssprintf.h"

#define TEST_I 15

bool autoGo = false;

struct MenuEntry
{
    const uint8_t start;
    const uint8_t numEntrys;
    void          (* funPtrNoArg)(void);
    void          (* funPtr)(Colors);
    const char*   desc;
};

static void testColor();
static void SortOne();
static void CheckAvailable();
static void DropBead();
static bool BeadAvailable();
static void testAll();
static void ClearQueue();
static void ToggleAutoGo();

static const MenuEntry menu[] =
{
    {1,  20, NULL,            &DropperSetPos, "Move dropper to position X"                },
    {51, 1,  &testColor,      NULL,           "Test the current color"                    },
    {52, 1,  &SortOne,        NULL,           "Sort one bead"                             },
    {53, 1,  &Agitate,        NULL,           "Agitate"                                   },
    {54, 1,  &DropBead,       NULL,           "Drop 1 bead"                               },
    {55, 1,  &CheckAvailable, NULL,           "Check if there are enough beads to proceed"},
    {56, 1,  &testAll,        NULL,           "Test all colors"                           },
    {57, 1,  &ClearQueue,     NULL,           "Empty the queue"                           },
    {99, 1,  &ToggleAutoGo,   NULL,           "Toggle automatic sorting"                  },
};

static void ToggleAutoGo()
{
    autoGo = !autoGo;
    ssprintf("> Autogo is now: %s", autoGo ? "ON" : "OFF");
}

static void CheckAvailable()
{
    ssprintf("> Bead is %s available.", BeadAvailable() ? "wel" : "NOT");
}

static void DropBead()
{
    Serial.println("Dropping");
    digitalWrite(PIN_SOLENOID, HIGH);
    delay(400);
    digitalWrite(PIN_SOLENOID, LOW);
}

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
        DropBead();
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

    for (const MenuEntry& entry : menu)
    {
        if (entry.numEntrys > 1)
        {
            ssprintf("%d - %d:", entry.start, entry.start + entry.numEntrys - 1);
        }
        else
        {
            ssprintf("%d:", entry.start);
        }
        Serial.println(entry.desc);
        Serial.println("-------------------------");
    }
}

static bool BeadAvailable()
{
    digitalWrite(PIN_AVAILABILITY_LED, HIGH);
    delay(50);
    int v = analogRead(PIN_AVAILABILITY_SENSOR);

    ssprintf("Availability sensor reads: %d", v);
    digitalWrite(PIN_AVAILABILITY_LED, LOW);
    return v > 650;
}

static void ClearQueue()
{
    Serial.println("CLEARIN QUEUE");
    DropperSetPos(Colors::DUMP);
    while (BeadAvailable())
    {
        DropBead();
        delay(600);
    }
    for (int i = 0; i < 10; i++)
    {
        DropBead();
        delay(600);
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
        delay(600);
        DropBead();
        delay(5000);
    }
}

static Colors TwoOutOFThree()
{
    while (true)
    {
        Colors c1 = TcsGetColor(TcsReadColor());
        Colors c2 = TcsGetColor(TcsReadColor());
        if (c1 == c2)
        {
            return c1;
        }
        Colors c3 = TcsGetColor(TcsReadColor());
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
    ssprintf("Dropping color: %s ", ColorToString(prevC));
    AgitatorUp();
    DropperSetPos(prevC);
    DropBead();
    prevC = c;
    AgitatorDown();
}

void loop()
{
    delay(100);
    if (digitalRead(PIN_BUTTON) == LOW || autoGo)
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
        if (state == 0)
        {
            continue;
        }

        ssprintf("New state = %d", state);

        bool found = false;
        for (const MenuEntry& entry : menu)
        {
            if (state >= entry.start && state < entry.start + entry.numEntrys)
            {
                if (entry.funPtr)
                {
                    entry.funPtr(static_cast < Colors > (state - 1));
                }
                else if (entry.funPtrNoArg)
                {
                    entry.funPtrNoArg();
                }
                found = true;
                break;
            }
        }

        if (!found)
        {
            ssprintf("> cannot execute command (%d)", state);
            printHelp();
        }
    }
}
