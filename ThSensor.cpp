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

#include "Dht22.h"
#include "ThSensor.h"

#define DHT22_PIN               2

static Dht22_Data_t dht22Data;
static ThSensor_Data_t sensorData;
static ThSensor_Stats_t sensorStats;

/*
 *
 */
void ThSensor_Init(void)
{
    Dht22_Init(DHT22_PIN);

    sensorData.dataValid = false;
    sensorData.t = 0;
    sensorData.rh = 0;

    sensorStats.totalCnt = 0;
    sensorStats.errorCnt = 0;
}

/*
 *
 */
void ThSensor_DoMeasure(void)
{
    uint8_t maxTry = 2;
    do
    {
        sensorStats.totalCnt++;
        Dht22_GetData(DHT22_PIN, &dht22Data);

        if(!dht22Data.dataValid)
        {
            sensorStats.errorCnt++;
        }

        maxTry--;
        if(maxTry == 0)
        {
            break;
        }

    } while (!dht22Data.dataValid);

    sensorData.dataValid = dht22Data.dataValid;
    sensorData.t = dht22Data.t;
    sensorData.rh = dht22Data.rh;
}

/*
 *
 */
const ThSensor_Data_t* ThSensor_GetData(void)
{
    return &sensorData;
}

/*
 *
 */
const ThSensor_Stats_t* ThSensor_GetStats(void)
{
    return &sensorStats;
}
