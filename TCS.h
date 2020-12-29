
#include <stdint.h>
#include <stdbool.h>

struct Color
{
    uint16_t red;
    uint16_t blue;
    uint16_t white;
    uint16_t green;
    uint16_t delta;
};

bool operator == (const Color& lhs, const Color& rhs);

void TcsInit();
struct Color TcsGetColor();
