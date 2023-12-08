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
