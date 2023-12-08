#ifndef LOGGER_H
#define LOGGER_H

#pragma pack(push, 1)

#define LOGGER_DUMMYIDX         0xffff

typedef struct Logger_Record_t
{
    uint32_t flagAndDt;
    int16_t t;
    uint8_t rh;

    // cs added by Ps

} Logger_Record_t;

#pragma pack(pop)

void Logger_Init(void);
void Logger_Periodic(void);
void Logger_Reset(void);
uint16_t Logger_GetRecord(uint16_t index, Logger_Record_t* record);

#endif
