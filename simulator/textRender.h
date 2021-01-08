#pragma once

#include <stdint.h>
#include <string>

#include "colors.h"
#include "windowSize.h"

void PrintText(Pixel pixels[heigth][width], uint16_t x, const uint16_t y, const Pixel &color, const std::string text);
void PrintfText(Pixel pixels[heigth][width], uint16_t x, const uint16_t y, const Pixel& color, const char* fmt, ...);
