#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

#define DEBUG_PIN           13

#define Debug_Init()        pinMode(DEBUG_PIN, OUTPUT);        \
                            digitalWrite(DEBUG_PIN, LOW);

#define Debug_SetLow()      digitalWrite(DEBUG_PIN, LOW)
#define Debug_SetHigh()     digitalWrite(DEBUG_PIN, HIGH)

#endif
