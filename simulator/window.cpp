#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <math.h>

#include "window.h"
#include "Arduino.h"
#include "textRender.h"

static GLFWwindow* window;
static Window* windowInstance;
static std::thread t;
static MainColor colors;

void thread_function()
{
    std::cout << "thread function 1\n";
    windowInstance->Thread();
}

void Window::Thread()
{
    std::cout << "thread function\n";

    /* Initialize the library */
    if (!glfwInit())
    {
        exit(1);
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, heigth, "Bead sorter simulator", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        Draw();
        glDrawPixels(width, heigth, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

Window::Window()
{
    Clear();
}

void Window::Clear()
{
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < heigth; h++)
        {
            pixels[h][w] = {255, 255, 255};
        }
    }
}

void Window::CloseWindow()
{
    t.join();
    delete windowInstance;
}

void Window::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel& color)
{
    if (abs(x2 - x1) > abs(y2 - y1))
    {
        if (x1 > x2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for (uint16_t x = x1; x < x2; x++)
        {
            const uint16_t y = (y2 - y1) * (x - x1) / (x2 - x1) + y1;
            pixels[y][x] = color;
        }
    }
    else
    {
        if (y1 > y2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for (uint16_t y = y1; y < y2; y++)
        {
            const uint16_t x = (x2 - x1) * (y - y1) / (y2 - y1) + x1;
            pixels[y][x] = color;
        }
    }
}

void Window::DrawBox(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const Pixel& color)
{
    if (x1 > x2)
    {
        std::swap(x1, x2);
    }
    if (y1 > y2)
    {
        std::swap(y1, y2);
    }
    for (uint16_t x = x1; x < x2; x++)
    {
        for (uint16_t y = y1; y < y2; y++)
        {
            pixels[y][x] = color;
        }
    }
}

void Window::DrawBox(uint16_t x, uint16_t y, const Pixel& color)
{
    DrawBox(x, y, x + 16, y + 16, color);
}

static const Pixel& ModeToColor(const PinMode& mode)
{
    switch (mode)
    {
        case UNSET:
            return colors.blue;

        case OUTPUT:
            return colors.green;

        case INPUT:
            return colors.red;

        case INPUT_PULLUP:
            return colors.orange;

        case SERVO:
            return colors.black;

        default:
            return colors.white;
    }
}

void Window::DrawPinStatus(uint16_t x, uint16_t y, uint8_t pin)
{
    const PinStatus& status = GetPinStatus(pin);

    PrintfText(pixels, x, y, colors.black, "%d", pin);
    uint8_t offset = 24;

    DrawBox(x + offset, y, ModeToColor(status.mode));
    offset += 16;
    DrawBox(x + offset, y, status.value > 0 ? colors.green : colors.red);
    offset += 16;
    for (uint16_t i = 0; i < status.value && i + offset < width; i++)
    {
        DrawLine(x + offset + i, y, x + offset + i, y + 16, colors.blue);
    }
}

static double degToRad(const double d)
{
    return (d * M_PI / 180);
}

void Window::FillRecursive(const uint16_t x, const uint16_t y, const Pixel oldColor, const Pixel &color, const uint16_t c)
{
    if (c == 0)
        return;
    if (pixels[y][x] == oldColor)
    {
        pixels[y][x] = color;
        if (x < width)
        {
            FillRecursive(x + 1, y, oldColor, color, c-1);
        }
        if (x > 0)
        {
            FillRecursive(x - 1, y, oldColor, color, c-1);
        }
        if (y < heigth)
        {
            FillRecursive(x, y + 1, oldColor, color, c-1);
        }
        if (y > 0)
        {
            FillRecursive(x, y - 1, oldColor, color, c-1);
        }
    }
    else
    {
        return;
    }
}

void Window::Fill(const uint16_t x, const uint16_t y, const Pixel &color)
{
    FillRecursive(x, y, pixels[y][x], color, 10000);
}

void Window::DrawCircle(const uint16_t x, const uint16_t y, const uint16_t radius, const uint16_t numberOfSides, const Pixel& color, bool fill)
{
    const double step     = 360.0 / numberOfSides;
    const uint16_t startx = x;
    const uint16_t starty = y + radius;
    uint16_t prevx        = startx;
    uint16_t prevy        = starty;

    for (uint16_t s = 1; s < numberOfSides; s++)
    {
        const double rad    = degToRad(s * step);
        const double sine   = sin(rad);
        const double cosine = cos(rad);
        const uint16_t sx   = x + sine * radius;
        const uint16_t sy   = y + cosine * radius;
        DrawLine(prevx, prevy, sx, sy, color);
        prevx = sx;
        prevy = sy;
    }
    DrawLine(prevx, prevy, startx, starty, color);

    if (fill)
    {
        Fill(x, y, colors.red);
    }
}

void Window::Draw(void)
{
    Clear();
    uint16_t y = 16;


    for (uint8_t i = 0; i < 20; i++)
    {
        DrawPinStatus(0, heigth - y, i);
        y += 16;
    }
    DrawCircle(300, 300, 100, 50, colors.orange);
}

void Window::OpenWindow(void)
{
    if (windowInstance == nullptr)
    {
        windowInstance = new Window();
    }

    t = std::thread(&thread_function);
}

Pixel::Pixel() :
    r(0),
    g(0),
    b(0)
{ }

Pixel::Pixel(GLubyte r, GLubyte g, GLubyte b) :
    r(r),
    g(g),
    b(b)
{ }
