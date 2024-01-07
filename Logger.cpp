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
#include <stdbool.h>

#include <Arduino.h>

#include "Ps.h"
#include "Rtc.h"
#include "ThSensor.h"
#include "Logger.h"

#pragma pack(push, 1)

typedef struct MetadataRecord_t
{
    uint8_t writeIndex;
    
    uint8_t numRecords;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t reserved4;
    uint8_t reserved5;
    uint8_t reserved6;

    // cs added by Ps

} MetadataRecord_t;

#pragma pack(pop)

static MetadataRecord_t metaRecord;
static const uint16_t recordOffset = sizeof(MetadataRecord_t) + 1;
static const uint8_t maxRecords = 120;

/*
 *
 */
static void saveRecord(const Logger_Record_t* record)
{
    uint16_t wrAddr = recordOffset +
                     metaRecord.writeIndex*(sizeof(Logger_Record_t)+1);

    Ps_Save(wrAddr, (const uint8_t*)record, sizeof(Logger_Record_t));

    metaRecord.writeIndex++;

    if(metaRecord.writeIndex == maxRecords)
    {
        metaRecord.writeIndex = 0;
    }

    if(metaRecord.numRecords < maxRecords)
    {
        metaRecord.numRecords++;
    }

    Ps_Save(0, (const uint8_t*)&metaRecord, sizeof(metaRecord));
}

/*
 *
 */
static void loggerReset(void)
{
    metaRecord.writeIndex = 0;
    metaRecord.numRecords = 0;

    metaRecord.reserved2 = 0;
    metaRecord.reserved3 = 0;
    metaRecord.reserved4 = 0;
    metaRecord.reserved5 = 0;
    metaRecord.reserved6 = 0;

    Ps_Save(0, (const uint8_t*)&metaRecord, sizeof(metaRecord));
}

/*
 *
 */
void Logger_Init(void)
{
    if(!Ps_Load(0, (uint8_t*)&metaRecord, sizeof(metaRecord)))
    {
        loggerReset();
        Ps_Load(0, (uint8_t*)&metaRecord, sizeof(metaRecord));
    }
}

/*
 *
 */
void Logger_Reset(void)
{
    loggerReset();
}

/*
 *
 */
void Logger_Periodic(void)
{
    const Rtc_ClockState_t* rtc = Rtc_GetRtcState();

    if(rtc->sec == 0)
    {
        if((rtc->min % 15) == 0)
        {
            Logger_Record_t newRecord;
            const ThSensor_Data_t* thSensorData = ThSensor_GetData();

            newRecord.flagAndDt = rtc->packed;
            if(thSensorData->dataValid)
            {
                newRecord.flagAndDt |= 1ul<<31;
            }

            newRecord.t = thSensorData->t * 10;
            newRecord.rh = thSensorData->rh;

            saveRecord(&newRecord);
        }
    }
}

/*
 *
 */
uint16_t Logger_GetRecord(uint16_t index, Logger_Record_t* record)
{
    uint16_t result = LOGGER_DUMMYIDX;

    if(index == LOGGER_DUMMYIDX)
    {
        index = metaRecord.writeIndex - 1;
        if(index == LOGGER_DUMMYIDX)
        {
            index = maxRecords - 1;
        }
    }

    if(index < maxRecords)
    {
        uint16_t rdAddr = recordOffset + index * (sizeof(Logger_Record_t)+1);

        if(!Ps_Load(rdAddr, (uint8_t*) record, sizeof(Logger_Record_t)))
        {
            record->flagAndDt = 0;
            record->t = 0;
            record->rh = 0;
        }

        if(index == metaRecord.writeIndex)
        {
            index = LOGGER_DUMMYIDX;
        }
        else
        {
            index--;
            if(index == LOGGER_DUMMYIDX)
            {
                index = maxRecords - 1;
            }
        }

        result = index;
    }
    else
    {
        record->flagAndDt = 0;
    }

    return result;
}
