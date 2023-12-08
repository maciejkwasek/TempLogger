#ifndef RTC_H
#define RTC_H

typedef struct Rtc_ClockState_t
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;

    uint8_t day;
    uint8_t month;
    uint16_t year;

    uint32_t packed;

} Rtc_ClockState_t;

void Rtc_Init(void);
bool Rtc_Periodic(void);
const Rtc_ClockState_t* Rtc_GetRtcState(void);
bool Rtc_SetRtcState(const Rtc_ClockState_t* newClockState);
void Rtc_UnpackTime(uint32_t dt, Rtc_ClockState_t* state);

#endif
