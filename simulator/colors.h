#include <GL/gl.h>
#include <iostream>

#pragma once

struct Pixel
{
    Pixel();
    Pixel(GLubyte r, GLubyte g, GLubyte b);

    GLubyte r;
    GLubyte g;
    GLubyte b;
};

bool operator== (const Pixel& lhs, const Pixel& rhs);

std::ostream& operator << (std::ostream& os, const Pixel& p);

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
