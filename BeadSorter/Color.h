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
    uint16_t redDelta;
    uint16_t blueDelta;
    uint16_t whiteDelta;
    uint16_t greenDelta;
};

enum Colors
{
    NONE,
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
    DUMP,

    PALE_GREEN,
    PALE_YELLOW,
    PALE_SKIN,
    PALE_BLUE,
    PALE_PURPLE
};

static const Color colors[] =
{
    // R B  W  G  D
    {76,   93,   65,   92,   11,  3,   22,  5 }, // NONE,
    {5364, 8484, 5474, 8896, 212, 448, 310, 44}, // RED,
    {5778, 5416, 5810, 5424, 882, 28,  846, 32}, // GREEN,
    {59,   43,   54,   40,   0,   3,   5,   5 }, // BLUE,
    {3940, 3654, 4006, 4470, 696, 854, 754, 46}, // YELLOW,
    {0,    0,    0,    0,    defdelta},          // ORANGE,
    {0,    0,    0,    0,    defdelta},          // PINK,
    {0,    0,    0,    0,    defdelta},          // PURPLE,
    {0,    0,    0,    0,    defdelta},          // GREY,
    {0,    0,    0,    0,    defdelta},          // BLACK,
    {0,    0,    0,    0,    defdelta},          // WHITE,
    {0,    0,    0,    0,    defdelta},          // GLOW,
    {0,    0,    0,    0,    defdelta},          // PALE_GREEN,
    {0,    0,    0,    0,    defdelta},          // PALE_YELLOW,
    {0,    0,    0,    0,    defdelta},          // DUMP,
};

const char* ColorToString(const Colors &color);
bool operator == (const Color& lhs, const Color& rhs);
