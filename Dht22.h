#ifndef DHT22_H
#define DHT22_H

typedef struct Dht22_Data_s
{
    bool dataValid;
    float rh;
    float t;

} Dht22_Data_t;

void Dht22_Init(int pin);
void Dht22_GetData(int pin, Dht22_Data_t* d);

#endif