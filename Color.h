
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define defdelta 10

struct Color
{
    uint16_t red;
    uint16_t blue;
    uint16_t white;
    uint16_t green;
    uint16_t delta;
};

enum Colors
{
    NONE,
    RED,  //1
    GREEN,
    BLUE,
    YELLOW,
    ORANGE, //5
    PINK,
    PURPLE,
    GREY,
    BLACK,
    WHITE, //10
    GLOW,
    PALE_GREEN,
    PALE_YELLOW,
    DUMP, //14
};

static const Color colors[] =
{
    // R B  W  G  D
    {0, 0, 0, 0, defdelta}, // NONE,
    {0, 0, 0, 0, defdelta}, // RED,
    {0, 0, 0, 0, defdelta}, // GREEN,
    {0, 0, 0, 0, defdelta}, // BLUE,
    {0, 0, 0, 0, defdelta}, // YELLOW,
    {0, 0, 0, 0, defdelta}, // ORANGE,
    {0, 0, 0, 0, defdelta}, // PINK,
    {0, 0, 0, 0, defdelta}, // PURPLE,
    {0, 0, 0, 0, defdelta}, // GREY,
    {0, 0, 0, 0, defdelta}, // BLACK,
    {0, 0, 0, 0, defdelta}, // WHITE,
    {0, 0, 0, 0, defdelta}, // GLOW,
    {0, 0, 0, 0, defdelta}, // PALE_GREEN,
    {0, 0, 0, 0, defdelta}, // PALE_YELLOW,
    {0, 0, 0, 0, defdelta}, // DUMP,
};

void ColorPrintName(Colors& color);
bool operator == (const Color& lhs, const Color& rhs);

