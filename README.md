# TempLogger

## What is it ?
TempLogger is a minimalistic and simple temperature and humidity logger.
All you need is arduino board (e.x. ArduinoNano) and DHT22 sensor.
You can use optional HC-06 bluetooth module in order to get remote access
to console of TempLogger.

![TempLogger](docs/tl1.jpg?raw=true "TempLogger")

## Diagram

![Connection diagram](docs/ConnDia.svg?raw=true "ConnDia")

## How does it work ?
TempLogger every 15 minutes measures current temperature and humidity
and stores new record of data in embedded EEPROM. The record of data 
consists: data and time, temperature and humidity. Each record is
protected by checksum against memory corruption. Because the ATmega328
eeprom is not big, there is a room only for 120 records. Old records are
overriding by new data. Measurement interval is hardcoded, and can be
changed easily.

## Console
Soon.
