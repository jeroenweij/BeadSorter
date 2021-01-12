#pragma once
#include <thread>

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
    void DrawCircle(const uint16_t x, const uint16_t y, const uint16_t radius, const uint16_t numberOfSides, const Pixel &color, bool fill = false);
    void Fill(const uint16_t x, const uint16_t y, const Pixel &color);
private:
    void Draw();
    void FillRecursive(const uint16_t x, const uint16_t y, const Pixel oldColor, const Pixel &color, const uint16_t c);
    void DrawDropper(const uint16_t x, const uint16_t y, const uint16_t radius, const uint16_t deg, const bool left, const Pixel &color);

    void Clear();
    void DrawDropPoint(const uint16_t x, const uint16_t y, const uint8_t id, const Pixel &color);

    Pixel pixels[heigth][width];
    static std::thread t;
};
