#pragma once
#define DEC 0

#include <stdint.h>
#include <string>

class Serial_ {
public:
    Serial_();
    void begin(int32_t speed);
    bool available();
    int32_t parseInt();

    void print(const std::string &s);
    void print(const char[]);
//    void print(char);
//    void print(unsigned char, int = DEC);
    void print(int, int = DEC);
//    void print(unsigned int, int = DEC);
//    void print(long, int = DEC);
//    void print(unsigned long, int = DEC);
//    void print(double, int = 2);

    void println(const std::string &s);
    void println(const char[]);
//    void println(char);
//    void println(unsigned char, int = DEC);
    void println(int, int = DEC);
//    void println(unsigned int, int = DEC);
//    void println(long, int = DEC);
//    void println(unsigned long, int = DEC);
//    void println(double, int = 2);
    void println(void);

    operator bool() const
    {
        return true; // Or false!
    }
};
