#include <stdint.h>

#include <Arduino.h>

#include "CmdLogger.h"
#include "Logger.h"
#include "Rtc.h"
#include "Utils.h"

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
            
            Rtc_ClockState_t clk;
            Rtc_UnpackTime(record.flagAndDt & 0x7fffffff, &clk);

            if(record.flagAndDt & 0x80000000)
            {
                Serial.print("+;");
            }
            else
            {
                Serial.print("-;");
            }

            Serial.print(clk.year); Serial.print(".");
            Serial.print(clk.month); Serial.print(".");
            Serial.print(clk.day); Serial.print(";");

            Serial.print(clk.hour); Serial.print(":");
            Serial.print(clk.min); Serial.print(":");
            Serial.print(clk.sec); Serial.print(";");

            Serial.print(record.t /10.0); Serial.print(";");
            Serial.print(record.rh); Serial.print(";");

            Serial.println("");
        }

    } while (index != LOGGER_DUMMYIDX);
}
