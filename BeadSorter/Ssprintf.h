#pragma once

#include <unistd.h>
#include <stdarg.h>

#define PRINT_BUF 100
void inline ssprintf(const char* fmt, ...)
{
    static char buf[PRINT_BUF];
    va_list va;

    va_start(va, fmt);
    vsnprintf(buf, PRINT_BUF, fmt, va);
    va_end(va);

    Serial.println(buf);
}
