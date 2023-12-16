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
