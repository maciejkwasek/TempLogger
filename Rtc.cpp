#include <stdint.h>
#include <stdbool.h>

#include "Rtc.h"
#include "RtcHal.h"

static uint8_t second; // 0..59 - 6bit
static uint8_t minute; // 0..59 - 6bit
static uint8_t hour;   // 0..23 - 5bit

static uint8_t day;    // 1..31(30,28,29) - 5bit
static uint8_t month;  // 1..12 - 4 bit
static uint16_t year;   // 1970..

const static uint8_t maxDayByMonth[] =
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static Rtc_ClockState_t clockState;

/*
 *
 */
static uint8_t getMaxDay(uint8_t m, uint8_t y)
{
    uint8_t result = maxDayByMonth[m-1];

    if(m == 2)
    {
        if((y % 4)==0)
        {
            if((y % 100)==0)
            {
                if((y % 400)==0)
                {
                    result++;
                }
            }
            else
            {
                result++;
            }
        }
    }

    return result;
}

/*
 *
 */
static uint32_t getPackedState(void)
{
    uint32_t result = (uint32_t) minute |
                      (uint32_t) hour << 6 |
                      (uint32_t) day << 11 |
                      ((uint32_t) month) << 16 |
                      (((uint32_t) (year-1970))& 0x000000ff) << 20;

    return result;
}

/*
 *
 */
void updateClockState(void)
{
    clockState.sec = second;
    clockState.min = minute;
    clockState.hour = hour;
    clockState.day = day;
    clockState.month = month;
    clockState.year = year;

    clockState.packed = getPackedState();
}

/*
 *
 */
void updateClock(void)
{
    second++;
    if(second == 60)
    {
        second = 0;

        minute++;
        if(minute == 60)
        {
            minute = 0;

            hour++;
            if(hour = 24)
            {
                hour = 0;

                day++;
                if(day > getMaxDay(month, year))
                {
                    day = 1;

                    month++;
                    if(month == 12)
                    {
                        month = 1;
                        year++;
                    }
                }
            }
        }
    }
}

/*
 *
 */
void Rtc_Init(void)
{
    second = 0;
    minute = 0;
    hour = 0;

    day = 1;
    month = 1;
    year = 1970;

    RtcHal_Init();
}

/*
 *
 */
bool Rtc_Periodic(void)
{
    bool result = false;

    if(RtcHal_IsOneSecondEvent())
    {
        updateClock();
        updateClockState();
        result = true;
    }

    return result;
}

/*
 *
 */
const Rtc_ClockState_t* Rtc_GetRtcState(void)
{
    return (const Rtc_ClockState_t*) &clockState;
}

/*
 *
 */
bool Rtc_SetRtcState(const Rtc_ClockState_t* newClockState)
{
    bool result = false;

    if(newClockState->hour < 60)
    {
        if(newClockState->min < 60)
        {
            if(newClockState->sec < 60)
            {
                if(newClockState->year >= 1970)
                {
                    if(newClockState->month >= 1 && newClockState->month <= 12)
                    {
                        uint8_t maxDay = getMaxDay(newClockState->month,
                                                   newClockState->year);

                        if(newClockState->day >= 1 &&
                           newClockState->day <= maxDay)
                        {
                            hour = newClockState->hour;
                            minute = newClockState->min;
                            second = newClockState->sec;

                            year = newClockState->year;
                            month = newClockState->month;
                            day = newClockState->day;

                            result = true;
                        }
                    }
                }
            }
        }
    }

    return result;
}

/*
 *
 */
void Rtc_UnpackTime(uint32_t dt, Rtc_ClockState_t* state)
{
    state->sec = 0;
    state->min = dt & 0x3ful;
    state->hour = (dt & (0x1ful << 6))>>6; 
    state->day = (dt & (0x1ful << 11))>>11;
    state->month = (dt & (0x0ful << 16))>>16;
    state->year = ((dt & (0xfful << 20))>>20) + 1970;

    state->packed = dt;
}
