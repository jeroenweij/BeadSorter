#include <stdarg.h>

#include "stdint.h"
#include "textRender.h"
#include "characters.h"

void PrintText(Pixel pixels[heigth][width], uint16_t x, const uint16_t y, const Pixel& color, const std::string text)
{
    for (const char c : text)
    {
        if (c >= ' ' && c <= '~')
        {
            Char ch = characters[c - ' '];
            for (const uint16_t d : ch.bits)
            {
                for (uint16_t y2 = 0; y2 < 16; y2++)
                {
                    if (d >> y2 & 0x01)
                    {
                        pixels[y + y2][x] = color;
                    }
                }
                x++;
            }
        }
    }
}

#define PRINT_BUF_LEN 100
void PrintfText(Pixel pixels[heigth][width], uint16_t x, const uint16_t y, const Pixel& color, const char* fmt, ...)
{
    static char buf[PRINT_BUF_LEN];
    va_list va;

    va_start(va, fmt);
    vsnprintf(buf, PRINT_BUF_LEN, fmt, va);
    va_end(va);

    PrintText(pixels, x,y,color,buf);
}
