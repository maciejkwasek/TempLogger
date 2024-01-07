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

#include <stdint.h>
#include <Arduino.h>

#include "Rtc.h"
#include "ThSensor.h"
#include "Logger.h"
#include "Console.h"

#include "Debug.h"

/*
 *
 */
void setup()
{
    Console_Init();
    Rtc_Init();
    ThSensor_Init();
    Logger_Init();

    Debug_Init();

    interrupts();
}

/*
 *
 */
void loop()
{
    if(Rtc_Periodic())
    {
        static uint8_t twoSecCnt = 0;

        twoSecCnt++;
        if(twoSecCnt == 2)
        {
            twoSecCnt = 0;

            Debug_SetHigh();
            ThSensor_DoMeasure();
            Debug_SetLow();
        }
        
        Logger_Periodic();
    }

    Console_Periodic();
}
