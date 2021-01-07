#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>

#include "window.h"
#include "Arduino.h"
#include "textRender.h"

static GLFWwindow* window;
static Window* windowInstance;
static std::thread t;
static MainColor color;

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
    if (x1 > x2)
    {
        std::swap(x1, x2);
    }
    if (y1 > y2)
    {
        std::swap(y1, y2);
    }
    if (x2 - x1 > y2 - y1)
    {
        for (uint16_t x = x1; x < x2; x++)
        {
            const uint16_t y = (y2 - y1) * (x - x1) / (x2 - x1) + y1;
            pixels[y][x] = color;
        }
    }
    else
    {
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
            return color.blue;

        case OUTPUT:
            return color.green;

        case INPUT:
            return color.red;

        case INPUT_PULLUP:
            return color.orange;

        case SERVO:
            return color.black;

        default:
            return color.white;
    }
}

void Window::DrawPinStatus(uint16_t x, uint16_t y, uint8_t pin)
{
    const PinStatus& status = GetPinStatus(pin);

    DrawBox(x, y, ModeToColor(status.mode));
    DrawBox(x + 16, y, status.value > 0 ? color.green : color.red);
    for (uint16_t i = 0; i < status.value && i + 32 < width; i++)
    {
        DrawLine(x + 32 + i, y, x + 32 + i, y + 16, color.blue);
    }
}

void Window::Draw(void)
{
    Clear();

    DrawLine(10, 40, 100, 450, {255, 0, 0});
    DrawLine(400, 100, 10, 450, {0, 255, 0});
    DrawLine(610, 450, 20, 100, {0, 0, 255});

    uint16_t y = 0;

    for (uint8_t i = 0; i < 20; i++)
    {
        DrawPinStatus(100, y, i);
        y += 16;
    }

    PrintText(pixels, 200, 400, color.black, "0123456789");
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
