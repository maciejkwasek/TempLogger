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

#include "CmdSensor.h"
#include "ThSensor.h"
#include "Logger.h"
#include "Rtc.h"
#include "Utils.h"

/*
 *
 */
void CmdSensor_Getdata(const char* params)
{
    UNUSED(params);

    const ThSensor_Data_t* thSensorData = ThSensor_GetData();

    if(thSensorData->dataValid)
    {
        char buffer[20];

        sprintf(buffer, "T=%d.%dC, Rh=%d%%",
            (int16_t)thSensorData->t, abs(((int16_t)(thSensorData->t*10))%10),
            (int16_t)thSensorData->rh
        );

        Serial.println(buffer);
    }
    else
    {
        Serial.println("sensor data not valid");
    }
}

/*
 *
 */
void CmdSensor_GetStats(const char* params)
{
    UNUSED(params);

    const ThSensor_Stats_t* thSensorStats = ThSensor_GetStats();

    float errorToTotal = (thSensorStats->errorCnt/thSensorStats->totalCnt)*100;

    Serial.print("total:");
    Serial.print(thSensorStats->totalCnt);
    Serial.print(", error:");
    Serial.print(thSensorStats->errorCnt);
    Serial.print(", error/total:");
    Serial.print((uint8_t)errorToTotal);
    Serial.print("%");

    Serial.println("");
}
