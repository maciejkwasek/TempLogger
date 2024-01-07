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
#include <stdio.h>

#include <Arduino.h>

#include "CmdTime.h"
#include "Rtc.h"
#include "Utils.h"

/*
 *
 */
void CmdTime_Settime(const char* params)
{
    uint32_t hour = 0;
    uint32_t min = 0;
    uint32_t sec = 0;

    uint32_t year = 0;
    uint32_t month = 0;
    uint32_t day = 0;

    /*
     * Params format: HH:MM:SS YYYY-MM-DD
     */

    if(sscanf(params, "%d.%d.%d %d:%d:%d",
              &year, &month, &day, &hour, &min, &sec)==6)
    {
        
        Rtc_ClockState_t newClockState;
        newClockState.hour = hour;
        newClockState.min = min;
        newClockState.sec = sec;
        newClockState.year = year;
        newClockState.month = month;
        newClockState.day = day;
        newClockState.packed = 0;
        
        if(Rtc_SetRtcState(&newClockState))
        {
            // Rtc set successfully
            Serial.println("set clock ok");
        }
        else
        {
            // Rtc set failed
            Serial.println("set clock failed");
        }
    }
    else
    {
        Serial.println("incorrect params format");
    }
}

/*
 *
 */
void CmdTime_Gettime(const char* params)
{
    UNUSED(params);

    const Rtc_ClockState_t* rtcState = Rtc_GetRtcState();
    char buffer[20];

    sprintf(buffer, "%04d.%02d.%02d %02d:%02d:%02d",
            rtcState->year, rtcState->month, rtcState->day,
            rtcState->hour, rtcState->min, rtcState->sec
    );

    Serial.println(buffer);
}
