#include "Serial.h"
#include <iostream>

Serial_::Serial_()
{ }

void Serial_::begin(int32_t speed)
{ }

bool Serial_::available()
{
    return false;
}

int32_t Serial_::parseInt()
{

}

void Serial_::print(const std::string &s)
{
    std::cout << s;
}

void Serial_::print(const char charArray[])
{
    std::cout << charArray;
}

void Serial_::print(int i, int)
{
    std::cout << i;
}

void Serial_::println(const std::string &s)
{
    std::cout << s << std::endl;
}

void Serial_::println(const char charArray[])
{
    std::cout << charArray << std::endl;
}

void Serial_::println(int i, int)
{
    std::cout << i << std::endl;
}

void Serial_::println()
{
    std::cout << std::endl;
}
