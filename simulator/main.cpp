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

static bool ColorTest()
{
    struct
    {
        Colors color;
        Color  rgb;
    } testColors[] = {
        {PALE_GREEN,  {3103, 3089, 2967, 3129}},
        {BROWN,       {2771, 2751, 2744, 2717}},
        {GLOW,        {3488, 3480, 3484, 3485}},
        {WHITE,       {3242, 3248, 3214, 3230}},
        {YELLOW,      {3337, 3211, 2820, 3148}},
        {ORANGE,      {3377, 3241, 2962, 3024}},
       // {BROWN,       {3377, 3241, 2962, 3024}},
       // {YELLOW,      {3377, 3241, 2962, 3024}},
        {PALE_BLUE,   {2904, 3037, 3066, 3030}},
        {YELLOW,      {3389, 3321, 3104, 3289}},
        {PURPLE,      {3141, 3138, 3199, 2945}},
        //{BROWN,       {3854, 358,  3766, 2821}},
        {PALE_GREEN,  {3239, 3243, 3163, 3259}},
        //{BROWN,       {2351, 2277, 2296, 2175}},
        {PALE_YELLOW, {3339, 3294, 3148, 3295}},
        {PALE_PURPLE, {2921, 2957, 2992, 2886}},
        {RED,         {3380, 3221, 2982, 2938}},
        //{PALE_SKIN,   {3176, 3129, 3050, 3094}},
        {PURPLE,      {3192, 3208, 3256, 3078}},
        {PALE_BLUE,   {2965, 3097, 3118, 3094}},
        //{BROWN,       {2331, 2290, 2198, 2207}},
        {BROWN,       {2930, 2929, 2916, 2891}},
        //{PALE_BLUE,   {3022, 3125, 3136, 3125}},
        {RED,         {3374, 3198, 2933, 2866}},
        //{GREY,        {3125, 3055, 2918, 3042}},
        {PINK,        {3442, 3408, 3376, 3262}},
        {GREEN,       {2849, 3068, 2769, 3258}},
        //{BLACK,       {4809, 1761, 1729, 4431}},
        //{GREY,        {3216, 3183, 3103, 3167}},
    };

    int i = 0;

    for (const auto& test : testColors)
    {
        Colors newColor = TcsGetColor(test.rgb);

        bool passed = (newColor == test.color);
        std::cout << "Color test: " << i << "  Got: " << ColorToString(newColor) << " Expected: " << ColorToString(test.color) << " test: " << (passed ? "Passed" : "FAILED") << std::endl;
        i++;
        if (!passed)
            return false;
    }
    return true;
}

static bool mainloopNoActios()
{
    WillReturn("digitalRead", 1);
    WillReturn("available", 0);

    loop();
    return true;
}

static bool mainloopManualInput()
{
    WillReturn("digitalRead", 0);
    WillReturn("analogRead", 1000); // beads available
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
    return true;
}

static bool mainloopSerialInput()
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
    WillReturn("analogRead", 1000);

    WillReturn("available", 1);
    WillReturn("analogRead", 100);
    WillReturn("parseInt", 21);

    WillReturn("available", 0);
    loop();
    return true;
}

int main()
{
    Window::OpenWindow();

    struct
    {
        std::string name;
        bool        (* testFunction)(void);
    } tests[] = {
        {"No actions",   &mainloopNoActios   },
        {"Serial input", &mainloopSerialInput},
        {"Manual input", &mainloopManualInput},
        {"Color test",   &ColorTest          },
    };

    setup();

    for (const auto& test : tests)
    {
        std::cout << std::endl << "Starting testcase: " << test.name << std::endl;
        bool ok = test.testFunction();

        std::cout << "Verifying testcase: " << test.name << std::endl;
        VerifyExpectations();

        std::cout << "TEST " << test.name << (ok?" PASSED":" FAILED") << std::endl;
    }

    Window::CloseWindow();
    return 0;
}
