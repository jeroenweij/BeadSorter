#pragma once

#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0
#define INPUT_PULLUP 0


#include <stdint.h>
#include <Serial.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

static Serial_ Serial;

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t pin, uint8_t val);
int16_t analogRead(uint8_t pin);
int16_t digitalRead(uint8_t pin);

void delay(uint32_t ms);

uint32_t pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 0);
uint32_t micros();

int32_t map(int32_t, int32_t, int32_t, int32_t, int32_t);



