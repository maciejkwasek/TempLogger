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
