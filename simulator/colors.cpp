#include "colors.h"

MainColor::MainColor() :
    black(0, 0, 0),
    white(255, 255, 255),
    red(255, 0, 0),
    green(0, 255, 0),
    blue(0, 0, 255),
    orange(255, 255, 0)
{ }

bool operator==(const Pixel &lhs, const Pixel &rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

std::ostream &operator <<(std::ostream &os, const Pixel &p)
{
    os << "RGB(" << (int)p.r << ", " << (int)p.g << ", " << (int)p.b << ")";
    return os;
}
