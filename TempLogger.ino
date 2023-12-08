#include <stdint.h>
#include <Arduino.h>

#include "Rtc.h"
#include "ThSensor.h"
#include "Logger.h"
#include "Console.h"

#include "Debug.h"

/*
 *
 */
void setup()
{
    Console_Init();
    Rtc_Init();
    ThSensor_Init();
    Logger_Init();

    Debug_Init();

    interrupts();
}

/*
 *
 */
void loop()
{
    if(Rtc_Periodic())
    {
        static uint8_t twoSecCnt = 0;

        twoSecCnt++;
        if(twoSecCnt == 2)
        {
            twoSecCnt = 0;

            Debug_SetHigh();
            ThSensor_DoMeasure();
            Debug_SetLow();
        }
        
        Logger_Periodic();
    }

    Console_Periodic();
}
