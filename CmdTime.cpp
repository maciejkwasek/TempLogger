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
