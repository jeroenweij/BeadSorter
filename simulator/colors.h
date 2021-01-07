#include <GL/gl.h>

#pragma once

struct Pixel
{
    Pixel();
    Pixel(GLubyte r, GLubyte g, GLubyte b);

    GLubyte r;
    GLubyte g;
    GLubyte b;
};

class MainColor {
public:
    MainColor();
    const Pixel black;
    const Pixel white;
    const Pixel red;
    const Pixel green;
    const Pixel blue;
    const Pixel orange;
};
