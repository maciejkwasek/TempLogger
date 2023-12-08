#include <stdint.h>

#include <Arduino.h>

#include "CmdPs.h"
#include "Ps.h"
#include "Logger.h"
#include "Utils.h"

/*
 *
 */
void CmdPs_Clear(const char* params)
{
    UNUSED(params);

    Ps_Clear();
    Logger_Reset();
    Serial.println("ee cleared");
}

/*
 *
 */
void CmdPs_Dump(const char* params)
{
    UNUSED(params);

    uint8_t buffer[8];

    for(uint16_t i=0; i<1024; i++)
    {
        sprintf((char*)buffer, "%02x ", Ps_GetByte(i));
        Serial.print((const char*)buffer);

        if(((i+1)%8) == 0)
        {
            Serial.println("");
        }
    }
}
