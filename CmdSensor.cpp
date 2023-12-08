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
        Serial.print("T=");
        Serial.print(thSensorData->t);
        Serial.print("C, Rh=");
        Serial.print(thSensorData->rh);
        Serial.println("%");
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

    Serial.print("total: ");
    Serial.print(thSensorStats->totalCnt);
    Serial.print(", error: ");
    Serial.print(thSensorStats->errorCnt);
    Serial.print(", error/total: ");
    Serial.print((((float)thSensorStats->errorCnt)/thSensorStats->totalCnt)*100);
    Serial.print("%");

    Serial.println("");
}
