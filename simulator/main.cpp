#include <iostream>

#include "Arduino.h"
#include "mock.h"
#include "window.h"
#include "BeadSorter.ino"

using namespace std;

static void ReturnFreq(int32_t f)
{
    WillReturn("pulseIn", 0);
    WillReturn("micros", 12);
    for (int i = 0; i < 250; i++)
    {
        WillReturn("pulseIn", 0);
    }
    WillReturn("micros", 12 + f);
}

static void mainloopNoActios()
{
    WillReturn("digitalRead", 1);
    WillReturn("available", 0);

    loop();
}

static void mainloopManualInput()
{
    WillReturn("digitalRead", 0);
    WillReturn("analogRead", 500); // beads available
    ReturnFreq(1234);
    ReturnFreq(2345);
    ReturnFreq(3456);
    ReturnFreq(4567);
    ReturnFreq(1234);
    ReturnFreq(2345);
    ReturnFreq(3456);
    ReturnFreq(4567);
    WillReturn("available", 0);

    loop();
}

static void mainloopSerialInput()
{
    WillReturn("digitalRead", 1);
    WillReturn("available", 1);
    WillReturn("parseInt", 0);

    WillReturn("available", 1);
    WillReturn("parseInt", 1);

    WillReturn("available", 1);
    WillReturn("parseInt", 14);

    WillReturn("available", 1);
    WillReturn("parseInt", 17);

    WillReturn("available", 1);
    WillReturn("parseInt", 18);

    WillReturn("available", 1);
    WillReturn("parseInt", 19);
    WillReturn("analogRead", 500);

    WillReturn("available", 1);
    WillReturn("parseInt", 21);

    WillReturn("available", 0);
    loop();
}

int main()
{
    Window::OpenWindow();

    struct
    {
        std::string name;
        void        (* testFunction)(void);
    } tests[] = {
        {"No actions",   &mainloopNoActios   },
        {"Serial input", &mainloopSerialInput},
        {"Manual input", &mainloopManualInput},
    };

    setup();

    for (const auto& test : tests)
    {
        std::cout << std::endl << "Starting testcase: " << test.name << std::endl;
        test.testFunction();

        std::cout << "Verifying testcase: " << test.name << std::endl;
        VerifyExpectations();
        std::cout << "TEST " << test.name << " PASSED" << std::endl;
    }

    Window::CloseWindow();
    return 0;
}
