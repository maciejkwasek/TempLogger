/*
 * MIT License
 *
 * Copyright (c) 2020 maciejkwasek
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Arduino.h>
#include <stdint.h>

#include "Dht22.h"

#define MAKE_UINT16(x, y)      ((uint16_t)((x))<<8 | ((y)))

const uint8_t dht22Timeout = 100;
const uint8_t dht22NumReadBits = 40;

/*
 *
 */
static void setLow(int pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

/*
 *
 */
static void setZ(int pin)
{
    pinMode(pin, INPUT_PULLUP);
}

/*
 *
 */
static uint8_t waitOnState(int pin, uint8_t blockState, uint8_t timeout)
{
    uint8_t t=0;

    while(digitalRead(pin)==blockState)
    {
        t++;
        if(t==timeout)
        {
            break;
        } 
    }

    return t;
}

/*
 *
 */
static bool evaluateBits(const  uint8_t* dataL, const uint8_t* dataH, uint8_t *b)
{
    uint8_t i;
    bool result = false;

    for(i=0; i<dht22NumReadBits; i++)
    {
        if(dataL[i] == dht22Timeout || dataH[i] == dht22Timeout)
        {
            break;
        }
        else
        {
            b[i/8] <<= 1;

            /* magic -5 */
            if((dataL[i]-5) < dataH[i])
            {
                b[i/8] |= 1;
            }
        }
    }

    if(i==dht22NumReadBits)
    {
        result = true;
    }

    return result;
}

/*
 *
 */
static bool checkIntegrity(const uint8_t* b)
{
    bool result = false;
    uint8_t calculatedCs = b[0] + b[1] + b[2] + b[3];

    if(calculatedCs == b[4])
    {
        result = true;
    }

    return result;
}

/*
 *
 */
void Dht22_Init(int pin)
{
    setZ(pin);
}

/*
 *
 */
void Dht22_GetData(int pin, Dht22_Data_t * d)
{
    uint8_t dataL[40];
    uint8_t dataH[40];

    uint8_t b[5] = {0, 0, 0, 0, 0};

    d->dataValid = false;

    // Start signal
    setLow(pin);
    delay(2);
    setZ(pin);
    delayMicroseconds(5);

    // ~ 5-6 @ 16MHz
    if(waitOnState(pin, HIGH, dht22Timeout) < dht22Timeout)
    {
        // Wait for response signal
        // ~ 25 @ 16MHz
        if(waitOnState(pin, LOW, dht22Timeout) < dht22Timeout)
        {
            // ~ 25 @ 16MHz
            if(waitOnState(pin, HIGH, dht22Timeout) < dht22Timeout)
            {
                for(uint8_t i=0; i<dht22NumReadBits; i++)
                {
                    // low ~ 16-20 @ 16MHz
                    // high ~ 6-7 lub 22-23 @ 16MHz
                    dataL[i] = waitOnState(pin, LOW, dht22Timeout);
                    dataH[i] = waitOnState(pin, HIGH, dht22Timeout);
                }

                if(evaluateBits(dataL, dataH, b))
                {
                    if(checkIntegrity(b))
                    {
                        d->dataValid = true;
                        d->rh = MAKE_UINT16(b[0], b[1])/ 10.0;
                        d->t = MAKE_UINT16(b[2] & 0x7f, b[3]) / 10.0;

                        if(b[2] & 0x80)
                        {
                            d->t *= -1; 
                        }
                    }
                }
            } 
        }
    }
}
