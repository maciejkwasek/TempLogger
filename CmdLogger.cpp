#include <stdint.h>

#include <Arduino.h>

#include "CmdLogger.h"
#include "Logger.h"
#include "Rtc.h"
#include "Utils.h"

/*
 *
 */
static void printRecord(const Logger_Record_t* record)
{
    Rtc_ClockState_t clk;
    Rtc_UnpackTime(record->flagAndDt & 0x7fffffff, &clk);

    char buffer[32];

    sprintf(buffer, "%c;%04d.%02d.%02d;%02d:%02d:%02d;%d.%d;%d",
        record->flagAndDt & 0x80000000 ? '+' : '-',
        clk.year, clk.month, clk.day,
        clk.hour, clk.min, clk.sec, 
        record->t/10, abs(record->t%10),
        record->rh
    );

    Serial.println(buffer);
}

/*
 *
 */
void CmdLogger_GetRecords(const char* params)
{
    UNUSED(params);

    Logger_Record_t record;
    uint16_t index = LOGGER_DUMMYIDX;

    do
    {
        index = Logger_GetRecord(index, &record);

        if(index != LOGGER_DUMMYIDX)
        {
            printRecord(&record);
        }

    } while (index != LOGGER_DUMMYIDX);
}
