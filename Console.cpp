#include <stdint.h>
#include <string.h>

#include <Arduino.h>

#include "CmdTime.h"
#include "CmdSensor.h"
#include "CmdPs.h"
#include "CmdLogger.h"

#include "Utils.h"

typedef void (*CmdHandler_t)(const char* params);

typedef struct CmdEntry_t
{
    const char* cmdString;
    CmdHandler_t handler;
} CmdEntry_t;


static void getdata(const char* params);

static uint8_t cmdBuffer[32];
static uint8_t cmdBufferIndex;

static const CmdEntry_t cmdEntries[] =
{
    {"settime", CmdTime_Settime},
    {"gettime", CmdTime_Gettime},
    {"getdata", CmdSensor_Getdata},
    {"eeclear", CmdPs_Clear},
    {"eedump", CmdPs_Dump},
    {"getrecords", CmdLogger_GetRecords},
    {"getstats", CmdSensor_GetStats}
};

/*
 *
 */
static void parseCmd(void)
{
    Serial.println((char*)cmdBuffer);

    uint8_t i = 0;
    for(i=0; i<ELEMS(cmdEntries); i++)
    {
        if(strncmp((const char*)cmdBuffer, cmdEntries[i].cmdString,
                   strlen(cmdEntries[i].cmdString))==0)
        {
            const char* params = strchr((const char*)cmdBuffer, ' ');
            if(params)
            {
                params++;
            }

            if(cmdEntries[i].handler)
            {
                cmdEntries[i].handler(params);
            }

            break;
        }
    }

    if(i == ELEMS(cmdEntries))
    {
        Serial.println("command not found");
    }
}

/*
 *
 */
void Console_Init(void)
{
    cmdBufferIndex = 0;

    Serial.begin(115200, SERIAL_8N1);
}

/*
 *
 */
void Console_Periodic(void)
{
    uint8_t c;

    int res = Serial.read();
    if(res >= 0)
    {
        c = res;
        Serial.write(c);

        if(c == '\n' || c == '\r')
        {
            cmdBuffer[cmdBufferIndex] = 0;
            cmdBufferIndex = 0;

            parseCmd();
        }
        else
        {
            cmdBuffer[cmdBufferIndex] = c;
            cmdBufferIndex++;
            if(sizeof(cmdBuffer) == cmdBufferIndex)
            {
                cmdBufferIndex = 0;
            }
        }
    }
}
