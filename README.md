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

### List of available commands
| Command | Description |
| ---          | --- |
| `gettime`    | Get current date and time. |
| `settime`    | Set current date and time. |
| `getdata`    | Get current measurments. |
| `getstats`   | Get measurement statistics. |
| `getrecords` | Get all records stored in internal eeprom. |
| `eeclear`    | Clear all records. |
| `eedump`     | Dump internal eeprom for debugging purposes. |

### `gettime`
Get current date and time.

#### Sample usage
```
gettime
2024.02.07 18:51:49
```
### `settime`
Set current date and time. Parameter format: `YYYY.MM.DD hh:mm:ss`

#### Sample usage
```
settime 2024.02.07 19:00:00
set clock ok
```
### `getdata`
Get current temperature in Celsius degrees and humidity in %.

#### Sample usage
```
getdata
T=22.8C, Rh=57%
```
### `getstats`
Get measurements statistic. 

#### Sample usage
```
getstats
total:1429, error:0, error/total:0%
```

### `getrecords`
Get all records stored in internal eeprom. Fields are separated by `;` (semicolon).
Each record has validity flag `+` or `-`, date, time, temperature in C, humidity in %.

#### Sample usage
```
getrecords
+;2024.01.09;12:00:00;-6.1;66
+;2024.01.09;11:45:00;-6.4;66
+;2024.01.09;11:30:00;-7.1;67
+;2024.01.09;11:15:00;-7.6;68
+;2024.01.09;11:00:00;-8.1;69
+;2024.01.09;10:45:00;-8.6;71
+;2024.01.09;10:30:00;-9.2;73
+;2024.01.09;10:15:00;-9.5;73
+;2024.01.09;10:00:00;-9.8;75
+;2024.01.09;09:45:00;-10.5;76
...
```

### `eeclear`
Clear all records and clear logger

#### Sample usage
```
eeclear
ee cleared
```
