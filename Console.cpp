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
