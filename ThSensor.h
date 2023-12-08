#ifndef THSENSOR_H
#define THSENSOR_H

typedef struct ThSensor_Data_t
{
    bool dataValid;
    float rh;
    float t;

} ThSensor_Data_t;

typedef struct ThSensor_Stats_t
{
    uint32_t totalCnt;
    uint32_t errorCnt;

} ThSensor_Stats_t;

void ThSensor_Init(void);
void ThSensor_DoMeasure(void);
const ThSensor_Data_t* ThSensor_GetData(void);
const ThSensor_Stats_t* ThSensor_GetStats(void);

#endif
