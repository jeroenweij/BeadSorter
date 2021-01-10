#pragma once

#include "colors.h"
#include "windowSize.h"

class Window {
public:
    Window();
    static void OpenWindow();
    static void CloseWindow();
    void Thread();

    void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel &color);
    void DrawBox(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel &color);
    void DrawBox(uint16_t x, uint16_t y, const Pixel &color);
    void DrawPinStatus(uint16_t x, uint16_t y, uint8_t pin);
    void DrawCircle(const uint16_t x, const uint16_t y, const uint16_t radius, const uint16_t numberOfSides, const Pixel &color, bool fill = true);
    void Fill(const uint16_t x, const uint16_t y, const Pixel &color);
private:
    void Draw();
    void FillRecursive(const uint16_t x, const uint16_t y, const Pixel oldColor, const Pixel &color, const uint16_t c);

    Pixel pixels[heigth][width];
    void Clear();
};
